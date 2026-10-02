/**
 * Disk/HDD Support - BIOS INT 13h + El Torito CD-ROM Boot
 * Based on Intel/Phoenix El Torito Specification
 */

#ifndef _DISK_H
#define _DISK_H

/* Basic types */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

/* BIOS Disk Functions */
#define DISK_RESET           0x00
#define DISK_READ_SECTORS   0x02
#define DISK_WRITE_SECTORS  0x03
#define DISK_VERIFY         0x04
#define DISK_GET_PARAMS     0x08
#define DISK_EXT_READ       0x42

/* Disk Status Codes */
#define DISK_SUCCESS         0x00
#define DISK_ERROR          0x01
#define DISK_NOT_FOUND      0x04
#define DISK_DRIVE_NOT_READY 0xAA
#define DISK_TIMEOUT        0x80

/* Bootable Media Types */
#define BOOT_MEDIA_HDD      0x80   /* Hard Disk (drive 0x80+) */
#define BOOT_MEDIA_FLOPPY   0x00   /* Floppy Disk (drive 0x00) */
#define BOOT_MEDIA_CDROM    0xFF   /* CD/DVD (El Torito) */

/* Partition Types */
#define PART_TYPE_EMPTY     0x00
#define PART_TYPE_FAT12     0x01
#define PART_TYPE_FAT16     0x04
#define PART_TYPE_EXTENDED  0x05
#define PART_TYPE_FAT16B    0x06
#define PART_TYPE_NTFS      0x07
#define PART_TYPE_FAT32     0x0B
#define PART_TYPE_FAT32_LBA 0x0C
#define PART_TYPE_EXTENDED_LBA 0x0F

/* MBR Signature */
#define MBR_SIGNATURE       0xAA55

#pragma pack(push, 1)

/* Partition Entry (16 bytes) */
typedef struct {
    uint8_t  status;         /* 0x80 = bootable, 0x00 = not */
    uint8_t  start_head;     /* Start head */
    uint8_t  start_cylinder; /* Start cylinder (10 bits) */
    uint8_t  start_sector;   /* Start sector (6 bits) */
    uint8_t  type;           /* Partition type */
    uint8_t  end_head;       /* End head */
    uint8_t  end_cylinder;   /* End cylinder (10 bits) */
    uint8_t  end_sector;     /* End sector (6 bits) */
    uint32_t lba_start;      /* LBA of start sector */
    uint32_t lba_count;      /* Number of sectors */
} partition_entry_t;

/* MBR (Master Boot Record) */
typedef struct {
    uint8_t           code[440];        /* Boot code */
    uint32_t          disk_signature;  /* Disk signature */
    uint16_t          reserved;        /* Usually 0x0000 */
    partition_entry_t partitions[4];   /* 4 partition entries */
    uint16_t          signature;       /* 0xAA55 */
} mbr_t;

/* ============================================================
 * El Torito CD-ROM Boot Structures
 * Sector 17 = Boot Record Volume Descriptor
 * Boot Catalog follows at LBA specified in BVD
 * ============================================================ */

/* Boot Record Volume Descriptor (Sector 17) */
typedef struct {
    uint8_t  type;             /* 0x00 = Boot Record */
    uint8_t  identifier[5];    /* "EL TORITO" */
    uint8_t  version;         /* 0x01 */
    uint8_t  system[32];      /* System identifier */
    uint8_t  unused1[32];     /* Unused */
    uint32_t lba_catalog;     /* LBA of boot catalog (little endian) */
    uint8_t  unused2[13];     /* Unused */
    uint16_t sig;             /* 0xAA55 */
} eltorito_bvd_t;

/* Boot Catalog - Validation Entry (at offset 0x00 of catalog sector) */
typedef struct {
    uint16_t sig;             /* 0xAA55 */
    uint8_t  platform_id;     /* 0x00=80x86, 0x01=PowerPC, 0x02=Mac */
    uint8_t  reserved1;       /* 0x00 */
    uint16_t reserved2;       /* 0x0000 */
    uint8_t  manufacturer[24];/* OEM identifier */
    uint16_t checksum;        /* Sector checksum */
    uint8_t  reserved3[26];    /* Reserved */
} eltorito_validation_t;

/* Boot Catalog - Initial/Default Boot Entry (at offset 0x20) */
typedef struct {
    uint8_t  boot_indicator;  /* 0x88 = bootable, 0x00 = not */
    uint8_t  media_type;       /* 0=no emulation, 1=1.2MB, 2=1.44MB, 3=2.88MB */
    uint8_t  load_segment;     /* Load segment (0x0000 = 0x7C0) */
    uint8_t  system_type;      /* System type (for no-emulation) */
    uint8_t  unused1;          /* Unused */
    uint16_t sector_count;     /* Sector count to load */
    uint32_t lba_start;        /* LBA of boot image (little endian) */
    uint8_t  unused2[20];      /* Unused */
} eltorito_boot_entry_t;

/* Section Header (for multiple boot entries) */
typedef struct {
    uint8_t  header_id;      /* 0x90 = header, 0x91 = last header */
    uint8_t  platform_id;     /* Platform ID */
    uint16_t num_entries;    /* Number of section entries */
    uint8_t  catalog_id[28]; /* Catalog identifier */
    uint16_t section_len;     /* Section length */
    uint16_t checks;         /* Header checksum */
} eltorito_section_header_t;

/* El Torito Media Types */
#define ELTORITO_NO_EMULATION   0x00
#define ELTORITO_1_2MB_FLOPPY  0x01
#define ELTORITO_1_44MB_FLOPPY 0x02
#define ELTORITO_2_88MB_FLOPPY 0x03
#define ELTORITO_HD_EMULATION   0x04

#pragma pack(pop)

/* Disk Functions */
int disk_reset(uint8_t drive);
int disk_read(uint8_t drive, uint32_t lba, uint8_t count, uint8_t *buffer);
int disk_get_params(uint8_t drive, uint8_t *heads, uint16_t *cylinders, uint8_t *sectors);

/* MBR/Partition Functions */
int disk_read_mbr(uint8_t drive, mbr_t *mbr);
int disk_read_partition(uint8_t drive, int partition_num, uint8_t *buffer);
int disk_get_partition_count(uint8_t drive);

/* Boot Functions */
void disk_boot_from_hdd(uint8_t drive);
void disk_boot_from_partition(uint8_t drive, int partition_num);
void disk_boot_from_floppy(void);
void disk_boot_from_cd(void);

/* El Torito Helper Functions */
int eltorito_find_boot_catalog(uint8_t drive, uint32_t *catalog_lba);
int eltorito_get_boot_entry(uint8_t drive, eltorito_boot_entry_t *entry);

#endif /* _DISK_H */
