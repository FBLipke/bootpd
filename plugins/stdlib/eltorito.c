/**
 * El Torito CD-ROM Boot Catalog Parser
 * Parses Boot Record Volume Descriptor, Boot Catalog, and Section Entry Extensions
 * Based on Intel/Phoenix El Torito Specification v1.0
 */

#include <stdlib.h>

/* El Torito structures are in disk.h/eltorito.h */

/* Find boot catalog LBA from Boot Record Volume Descriptor */
int eltorito_find_catalog(uint8_t drive, uint32_t *catalog_lba) {
    uint8_t buffer[2048];
    eltorito_bvd_t *bvd = (eltorito_bvd_t*)buffer;
    
    /* Read sector 17 - Boot Record Volume Descriptor */
    if (disk_read(drive, 17, 1, buffer) != DISK_SUCCESS) {
        return -1;
    }
    
    /* Check for El Torito signature */
    if (bvd->type != 0x00) {
        return -1;
    }
    if (bvd->identifier[0] != 'E' || bvd->identifier[1] != 'L') {
        return -1;
    }
    
    *catalog_lba = bvd->lba_catalog;
    return 0;
}

/* Verify checksum (16-bit words, should sum to 0) */
int eltorito_verify_checksum(uint16_t *data, int words) {
    uint16_t sum = 0;
    int i;
    for (i = 0; i < words; i++) {
        sum += data[i];
    }
    return (sum == 0);
}

/* Parse Section Entry Extensions (Header ID = 0x01)
 * These follow a Section Entry when more selection criteria is needed.
 * Bit 5 of platform_id indicates if more extensions follow. */
static int parse_extensions(uint8_t *buffer, int offset, eltorito_boot_info_t *entry) {
    eltorito_ext_t *ext;
    int ext_offset = 0;
    
    while (offset + ext_offset < 2040) {
        ext = (eltorito_ext_t*)(buffer + offset + ext_offset);
        
        /* Check for extension header (must be 0x01) */
        if (ext->header_id != ELTORITO_EXT_ENTRY) {
            break;  /* No more extensions */
        }
        
        /* Store platform from extension if not already set */
        if (entry->platform_id == 0) {
            entry->platform_id = ext->platform_id & 0x1F;  /* Mask bit 5 */
        }
        
        /* Check bit 5: 1 = more extensions follow, 0 = last extension */
        if (!(ext->platform_id & ELTORITO_EXT_FOLLOWS)) {
            break;  /* Last extension */
        }
        
        ext_offset += 32;  /* Move to next extension (32 bytes each) */
    }
    
    return ext_offset;
}

/* Parse entire boot catalog and fill entries array */
int eltorito_parse_catalog(uint8_t drive, eltorito_boot_info_t *entries, int max_entries) {
    uint8_t buffer[2048];
    eltorito_validation_t *validation;
    eltorito_entry_t *entry;
    eltorito_section_t *section;
    int entry_count = 0;
    int offset;
    
    /* Find and read boot catalog */
    uint32_t catalog_lba;
    if (eltorito_find_catalog(drive, &catalog_lba) != 0) {
        return -1;
    }
    
    if (disk_read(drive, catalog_lba, 1, buffer) != DISK_SUCCESS) {
        return -1;
    }
    
    /* Validate catalog */
    validation = (eltorito_validation_t*)buffer;
    if (validation->sig != 0xAA55) {
        return -1;
    }
    
    /* Parse Initial/Default Entry at offset 0x20 */
    entry = (eltorito_entry_t*)(buffer + 0x20);
    if (entry->boot_indicator == ELTORITO_BOOTABLE && entry_count < max_entries) {
        entries[entry_count].bootable = entry->boot_indicator;
        entries[entry_count].media_type = entry->media_type;
        entries[entry_count].load_segment = entry->load_segment;
        entries[entry_count].system_type = entry->system_type;
        entries[entry_count].sector_count = entry->sector_count;
        entries[entry_count].lba_start = entry->lba_start;
        entries[entry_count].image_size = entry->sector_count * 512;
        entries[entry_count].platform_id = validation->platform_id;
        
        /* Create label */
        if (entry->media_type == MEDIA_NO_EMULATION) {
            strcpy(entries[entry_count].label, "Boot Image");
        } else if (entry->media_type == MEDIA_1_44MB_FLOPPY) {
            strcpy(entries[entry_count].label, "Floppy 1.44MB");
        } else if (entry->media_type == MEDIA_HD_EMULATION) {
            strcpy(entries[entry_count].label, "Hard Disk");
        } else {
            strcpy(entries[entry_count].label, "Boot Entry");
        }
        entry_count++;
    }
    
    /* Parse Section Headers and additional entries */
    offset = 0x40;  /* After Initial/Default Entry */
    
    while (offset < 2040 && entry_count < max_entries) {
        section = (eltorito_section_t*)(buffer + offset);
        
        /* Check for section header (0x90 or 0x91) */
        if (section->header_id != ELTORITO_HEADER && 
            section->header_id != ELTORITO_LAST_HEADER) {
            break;  /* No more section headers */
        }
        
        /* Parse boot entries in this section */
        int i;
        for (i = 0; i < section->num_entries && entry_count < max_entries; i++) {
            int entry_offset = offset + 32 + (i * 32);
            entry = (eltorito_entry_t*)(buffer + entry_offset);
            
            if (entry->boot_indicator == ELTORITO_BOOTABLE) {
                entries[entry_count].bootable = entry->boot_indicator;
                entries[entry_count].media_type = entry->media_type;
                entries[entry_count].load_segment = entry->load_segment;
                entries[entry_count].system_type = entry->system_type;
                entries[entry_count].sector_count = entry->sector_count;
                entries[entry_count].lba_start = entry->lba_start;
                entries[entry_count].image_size = entry->sector_count * 512;
                entries[entry_count].platform_id = section->platform_id;
                
                /* Use catalog_id as label if available */
                if (section->catalog_id[0] != 0) {
                    int j;
                    for (j = 0; j < 28 && j < 31; j++) {
                        entries[entry_count].label[j] = section->catalog_id[j];
                    }
                    entries[entry_count].label[31] = '\0';
                } else {
                    strcpy(entries[entry_count].label, "Boot Entry");
                }
                
                /* Check for Section Entry Extensions after this entry */
                int ext_offset = entry_offset + 32;
                parse_extensions(buffer, ext_offset, &entries[entry_count]);
                
                entry_count++;
            }
        }
        
        offset += 32;  /* Move past section header */
        
        /* If last header (0x91), stop */
        if (section->header_id == ELTORITO_LAST_HEADER) {
            break;
        }
    }
    
    return entry_count;
}

/* Get single boot entry by index */
int eltorito_get_entry(uint8_t drive, int index, eltorito_boot_info_t *entry) {
    eltorito_boot_info_t temp_entries[MAX_BOOT_ENTRIES];
    int count = eltorito_parse_catalog(drive, temp_entries, MAX_BOOT_ENTRIES);
    
    if (index < 0 || index >= count) {
        return -1;
    }
    
    entry->bootable = temp_entries[index].bootable;
    entry->media_type = temp_entries[index].media_type;
    entry->load_segment = temp_entries[index].load_segment;
    entry->system_type = temp_entries[index].system_type;
    entry->sector_count = temp_entries[index].sector_count;
    entry->lba_start = temp_entries[index].lba_start;
    entry->image_size = temp_entries[index].image_size;
    entry->platform_id = temp_entries[index].platform_id;
    
    int j;
    for (j = 0; j < 32; j++) {
        entry->label[j] = temp_entries[index].label[j];
    }
    
    return 0;
}

/* Show boot menu and wait for selection */
void eltorito_show_menu(eltorito_boot_info_t *entries, int count) {
    int i;
    
    printf("\n");
    printf("========================================\n");
    printf("      CD-ROM Boot Menu\n");
    printf("========================================\n");
    
    for (i = 0; i < count; i++) {
        printf("  %d. %s", i + 1, entries[i].label);
        
        /* Show media type */
        if (entries[i].media_type == MEDIA_NO_EMULATION) {
            printf(" (No Emulation)");
        } else if (entries[i].media_type == MEDIA_1_44MB_FLOPPY) {
            printf(" (1.44MB Floppy)");
        } else if (entries[i].media_type == MEDIA_HD_EMULATION) {
            printf(" (Hard Disk)");
        }
        
        /* Show platform */
        if (entries[i].platform_id == PLATFORM_POWERPC) {
            printf(" [PowerPC]");
        } else if (entries[i].platform_id == PLATFORM_MAC) {
            printf(" [Mac]");
        }
        
        printf("\n");
    }
    
    printf("\n");
    printf("  0. Boot from first HDD\n");
    printf("\n");
    printf("========================================\n");
    printf("Select boot entry: ");
}

/* Boot from selected entry */
void eltorito_boot_entry(eltorito_boot_info_t *entry) {
    uint8_t *load_addr;
    uint16_t load_seg;
    
    /* Determine load address (x86 segment:offset) */
    if (entry->load_segment == 0) {
        load_seg = 0x07C0;  /* Traditional: 0x07C0:0x0000 = 0x7C00 */
    } else {
        load_seg = entry->load_segment;
    }
    load_addr = (uint8_t*)(load_seg << 4);  /* Convert segment to linear address */
    
    /* Load boot image */
    printf("Loading from LBA %u...\n", entry->lba_start);
    
    if (disk_read(0xFF, entry->lba_start, entry->sector_count, load_addr) != DISK_SUCCESS) {
        printf("ERROR: Failed to read boot image!\n");
        return;
    }
    
    printf("Jumping to %04x:%04x...\n", load_seg, 0);
    
    /* Jump to boot image */
    if (entry->media_type == MEDIA_NO_EMULATION) {
        /* No emulation - jump directly to load address */
        ((void (*)(void))(load_seg << 4))();
    } else {
        /* Emulated - jump to 0:0x7C00 */
        ((void (*)(void))0x7C00)();
    }
}
