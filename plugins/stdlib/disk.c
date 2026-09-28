/**
 * Disk/HDD Support - BIOS INT 13h + El Torito CD-ROM Boot
 * Based on Intel/Phoenix El Torito Specification v1.0 (1995)
 */

#include <stdlib.h>

/* BIOS INT 13h wrapper */
static uint8_t disk_int13(uint8_t ah, uint8_t al, uint16_t cx, uint16_t dx, uint16_t es, uint16_t bx) {
    uint8_t status;
    
    __asm__ __volatile__ (
        "movb %[ah], %%ah\n"
        "movb %[al], %%al\n"
        "movw %[cx], %%cx\n"
        "movw %[dx], %%dx\n"
        "movw %[es], %%es\n"
        "movw %[bx], %%bx\n"
        "int $0x13\n"
        "movb %%ah, %[status]\n"
        : [status] "=m" (status)
        : [ah] "m" (ah), [al] "m" (al),
          [cx] "m" (cx), [dx] "m" (dx),
          [es] "m" (es), [bx] "m" (bx)
        : "eax", "ebx", "ecx", "edx"
    );
    
    return status;
}

/* Reset disk system */
int disk_reset(uint8_t drive) {
    return disk_int13(DISK_RESET, 0, 0, drive, 0, 0);
}

/* Read sectors using CHS */
int disk_read_chs(uint8_t drive, uint8_t head, uint16_t cylinder, 
                   uint8_t sector, uint8_t count, uint8_t *buffer) {
    uint16_t es = ((uint32_t)buffer >> 4) & 0xFFFF;
    uint16_t bx = (uint32_t)buffer & 0xFFFF;
    
    uint16_t cx = (cylinder << 8) | ((cylinder & 0x300) >> 2) | sector;
    
    return disk_int13(DISK_READ_SECTORS, count, cx, (head << 8) | drive, es, bx);
}

/* Read sectors using LBA (Extended INT 13h) */
int disk_read_lba(uint8_t drive, uint32_t lba, uint8_t count, uint8_t *buffer) {
    uint16_t es = ((uint32_t)buffer >> 4) & 0xFFFF;
    uint16_t bx = (uint32_t)buffer & 0xFFFF;
    
    /* DAPA (Disk Address Packet) on stack */
    uint16_t dapa[8];
    dapa[0] = 16;           /* size */
    dapa[1] = 0;            /* reserved */
    dapa[2] = count;        /* sector count */
    dapa[3] = 0;            /* reserved */
    dapa[4] = bx;           /* offset */
    dapa[5] = es;           /* segment */
    dapa[6] = lba & 0xFFFF;        /* LBA low */
    dapa[7] = (lba >> 16) & 0xFFFF; /* LBA high */
    
    uint8_t status;
    __asm__ __volatile__ (
        "movb $0x42, %%ah\n"
        "movb %[drive], %%dl\n"
        "movw %[dapa], %%si\n"
        "int $0x13\n"
        "movb %%ah, %[status]\n"
        : [status] "=m" (status)
        : [drive] "m" (drive), [dapa] "m" (dapa[0])
        : "eax", "esi"
    );
    
    return status;
}

/* Read sectors - auto-select LBA or CHS */
int disk_read(uint8_t drive, uint32_t lba, uint8_t count, uint8_t *buffer) {
    /* Try LBA first */
    if (disk_read_lba(drive, lba, count, buffer) == DISK_SUCCESS) {
        return DISK_SUCCESS;
    }
    
    /* Fallback to CHS */
    uint8_t sector = (lba % 63) + 1;
    uint8_t head = (lba / 63) % 255;
    uint16_t cylinder = lba / (63 * 255);
    
    return disk_read_chs(drive, head, cylinder, sector, count, buffer);
}

/* Get drive parameters */
int disk_get_params(uint8_t drive, uint8_t *heads, uint16_t *cylinders, uint8_t *sectors) {
    uint16_t cx = 0;
    
    __asm__ __volatile__ (
        "movb $0x08, %%ah\n"
        "int $0x13\n"
        "movw %%cx, %[cx]\n"
        : [cx] "=m" (cx)
        : 
        : "eax", "ebx"
    );
    
    *sectors = (cx & 0x3F);
    *cylinders = (cx >> 6) & 0x3FF;
    
    uint8_t dh = 0;
    __asm__ __volatile__ (
        "movb $0x08, %%ah\n"
        "int $0x13\n"
        "movb %%dh, %[dh]\n"
        : [dh] "=m" (dh)
        :
        : "eax"
    );
    
    *heads = dh + 1;
    
    return 0;
}

/* Read MBR */
int disk_read_mbr(uint8_t drive, mbr_t *mbr) {
    int ret = disk_read(drive, 0, 1, (uint8_t*)mbr);
    if (ret != DISK_SUCCESS) {
        return ret;
    }
    
    if (mbr->signature != MBR_SIGNATURE) {
        return -1;
    }
    
    return 0;
}

/* Boot from HDD - find active partition and boot */
void disk_boot_from_hdd(uint8_t drive) {
    uint8_t *boot_sector = (uint8_t*)0x7C00;
    
    if (disk_read(drive, 0, 1, boot_sector) != DISK_SUCCESS) {
        return;
    }
    
    if (boot_sector[510] != 0x55 || boot_sector[511] != 0xAA) {
        return;
    }
    
    mbr_t *mbr = (mbr_t*)boot_sector;
    int i;
    for (i = 0; i < 4; i++) {
        if (mbr->partitions[i].status == 0x80) {
            uint32_t lba = mbr->partitions[i].lba_start;
            
            if (disk_read(drive, lba, 1, boot_sector) == DISK_SUCCESS) {
                ((void (*)(void))0x7C00)();
            }
        }
    }
    
    /* No active partition - try MBR code anyway */
    ((void (*)(void))0x7C00)();
}

/* Boot from specific partition */
void disk_boot_from_partition(uint8_t drive, int partition_num) {
    uint8_t *boot_sector = (uint8_t*)0x7C00;
    
    if (partition_num < 0 || partition_num > 3) {
        return;
    }
    
    mbr_t mbr;
    if (disk_read_mbr(drive, &mbr) != 0) {
        return;
    }
    
    partition_entry_t *part = &mbr.partitions[partition_num];
    
    if (part->type == PART_TYPE_EMPTY || part->lba_count == 0) {
        return;
    }
    
    if (disk_read(drive, part->lba_start, 1, boot_sector) != DISK_SUCCESS) {
        return;
    }
    
    if (boot_sector[510] != 0x55 || boot_sector[511] != 0xAA) {
        return;
    }
    
    ((void (*)(void))0x7C00)();
}

/* Boot from floppy */
void disk_boot_from_floppy(void) {
    uint8_t *boot_sector = (uint8_t*)0x7C00;
    
    if (disk_read(0x00, 0, 1, boot_sector) != DISK_SUCCESS) {
        return;
    }
    
    if (boot_sector[510] != 0x55 || boot_sector[511] != 0xAA) {
        return;
    }
    
    ((void (*)(void))0x7C00)();
}

/* Boot from CD/DVD using El Torito */
void disk_boot_from_cd(void) {
    uint8_t buffer[2048];
    eltorito_bvd_t *bvd;
    eltorito_validation_t *validation;
    eltorito_boot_entry_t *boot_entry;
    uint32_t catalog_lba;
    
    /* Read sector 17 - Boot Record Volume Descriptor */
    if (disk_read(0xFF, 17, 1, buffer) != DISK_SUCCESS) {
        return;
    }
    
    bvd = (eltorito_bvd_t*)buffer;
    
    /* Check for El Torito signature */
    if (bvd->type != 0x00) {
        return;
    }
    if (bvd->identifier[0] != 'E' || bvd->identifier[1] != 'L') {
        return;
    }
    
    catalog_lba = bvd->lba_catalog;
    
    /* Read boot catalog */
    if (disk_read(0xFF, catalog_lba, 1, buffer) != DISK_SUCCESS) {
        return;
    }
    
    validation = (eltorito_validation_t*)buffer;
    
    /* Validate boot catalog signature */
    if (validation->sig != 0xAA55) {
        return;
    }
    
    /* First entry after validation (at offset 0x20) is the boot entry */
    boot_entry = (eltorito_boot_entry_t*)(buffer + 32);
    
    /* Check if bootable */
    if (boot_entry->boot_indicator != 0x88) {
        return;
    }
    
    /* Determine load address */
    uint16_t load_seg = boot_entry->load_segment;
    if (load_seg == 0) {
        load_seg = 0x7C0;  /* Default for CD boot */
    }
    
    uint8_t *load_addr = (uint8_t*)(load_seg << 4);
    uint16_t sectors = boot_entry->sector_count;
    
    if (sectors == 0) {
        sectors = 1;
    }
    
    /* Read boot image */
    if (disk_read(0xFF, boot_entry->lba_start, sectors, load_addr) != DISK_SUCCESS) {
        return;
    }
    
    /* Jump to boot image based on media type */
    if (boot_entry->media_type == ELTORITO_NO_EMULATION) {
        /* No emulation - jump to loaded image */
        ((void (*)(void))(load_seg << 4))();
    } else {
        /* Emulated floppy/hdd - jump to 0:0x7C00 */
        ((void (*)(void))0x7C00)();
    }
}

/* Find boot catalog LBA */
int eltorito_find_boot_catalog(uint8_t drive, uint32_t *catalog_lba) {
    uint8_t buffer[2048];
    eltorito_bvd_t *bvd;
    
    /* Read sector 17 - Boot Record Volume Descriptor */
    if (disk_read(drive, 17, 1, buffer) != DISK_SUCCESS) {
        return -1;
    }
    
    bvd = (eltorito_bvd_t*)buffer;
    
    /* Check for El Torito */
    if (bvd->type != 0x00) {
        return -1;
    }
    if (bvd->identifier[0] != 'E' || bvd->identifier[1] != 'L') {
        return -1;
    }
    
    *catalog_lba = bvd->lba_catalog;
    return 0;
}

/* Get first boot entry from catalog */
int eltorito_get_boot_entry(uint8_t drive, eltorito_boot_entry_t *entry) {
    uint8_t buffer[2048];
    eltorito_validation_t *validation;
    
    /* Read boot catalog */
    uint32_t catalog_lba;
    if (eltorito_find_boot_catalog(drive, &catalog_lba) != 0) {
        return -1;
    }
    
    if (disk_read(drive, catalog_lba, 1, buffer) != DISK_SUCCESS) {
        return -1;
    }
    
    validation = (eltorito_validation_t*)buffer;
    
    if (validation->sig != 0xAA55) {
        return -1;
    }
    
    /* Copy boot entry from offset 0x20 */
    eltorito_boot_entry_t *src = (eltorito_boot_entry_t*)(buffer + 32);
    entry->boot_indicator = src->boot_indicator;
    entry->media_type = src->media_type;
    entry->load_segment = src->load_segment;
    entry->system_type = src->system_type;
    entry->sector_count = src->sector_count;
    entry->lba_start = src->lba_start;
    
    return 0;
}
