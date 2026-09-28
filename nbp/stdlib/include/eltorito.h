/**
 * El Torito CD-ROM Boot Catalog Parser
 * Supports Multiple Boot Entries + Section Entry Extensions
 */

#ifndef _ELTORITO_H
#define _ELTORITO_H

#include <addresses.h>

/* El Torito Boot Entry Types */
#define ELTORITO_BOOTABLE       0x88
#define ELTORITO_NOT_BOOTABLE   0x00

/* Section Header IDs */
#define ELTORITO_HEADER       0x90
#define ELTORITO_LAST_HEADER  0x91

/* Section Entry Extension ID */
#define ELTORITO_EXT_ENTRY    0x01

/* Platform IDs */
#define PLATFORM_80X86    0x00
#define PLATFORM_POWERPC  0x01
#define PLATFORM_MAC      0x02

/* Media Types */
#define MEDIA_NO_EMULATION  0x00
#define MEDIA_1_2MB_FLOPPY 0x01
#define MEDIA_1_44MB_FLOPPY 0x02
#define MEDIA_2_88MB_FLOPPY 0x03
#define MEDIA_HD_EMULATION  0x04

/* Extension bit */
#define ELTORITO_EXT_FOLLOWS  0x20  /* Bit 5 set = more extensions */

#pragma pack(push, 1)

/* Boot Catalog Entry - Generic */
typedef struct {
    uint8_t  boot_indicator;
    uint8_t  media_type;
    uint8_t  load_segment;
    uint8_t  system_type;
    uint8_t  unused1;
    uint16_t sector_count;
    uint32_t lba_start;
    uint8_t  unused2[20];
} eltorito_entry_t;

/* Section Header */
typedef struct {
    uint8_t  header_id;        /* 0x90 = more, 0x91 = last */
    uint8_t  platform_id;       /* 0x00=80x86, 0x01=PPC, 0x02=Mac */
    uint16_t num_entries;      /* Number of boot entries */
    uint8_t  catalog_id[28];   /* Descriptive name */
    uint16_t checksum;         /* Section checksum */
} eltorito_section_t;

/* Section Entry Extension (Header ID = 0x01) */
typedef struct {
    uint8_t  header_id;        /* Must be 0x01 */
    uint8_t  platform_id;      /* Platform for this extension */
    /* Additional selection criteria can follow */
    /* Up to 13 bytes total (same as section entry) */
} eltorito_ext_t;

/* Boot Entry with extended info */
typedef struct {
    char     label[32];        /* Display name */
    uint8_t  bootable;         /* 0x88 = bootable */
    uint8_t  media_type;       /* Emulation type */
    uint16_t load_segment;    /* Load segment */
    uint8_t  system_type;      /* System type (for no-emu) */
    uint16_t sector_count;    /* Sectors to load */
    uint32_t lba_start;       /* Where image starts */
    uint32_t image_size;       /* Calculated size */
    uint8_t  platform_id;      /* Platform (x86/PPC/Mac) */
} eltorito_boot_info_t;

#pragma pack(pop)

/* Max boot entries we support */
#define MAX_BOOT_ENTRIES 16

/* ========================================================================
 * EL TORITO PARSING FUNCTIONS
 * ======================================================================== */

/* Find boot catalog LBA from Boot Record Volume Descriptor */
int eltorito_find_catalog(uint8_t drive, uint32_t *catalog_lba);

/* Parse entire boot catalog and return number of entries */
int eltorito_parse_catalog(uint8_t drive, eltorito_boot_info_t *entries, int max_entries);

/* Get single boot entry by index */
int eltorito_get_entry(uint8_t drive, int index, eltorito_boot_info_t *entry);

/* Calculate checksum of section */
int eltorito_verify_checksum(uint16_t *data, int words);

/* Display boot menu from entries */
void eltorito_show_menu(eltorito_boot_info_t *entries, int count);

/* Boot from selected entry */
void eltorito_boot_entry(eltorito_boot_info_t *entry);

/* Parse section entry extensions */
int eltorito_parse_extensions(uint8_t *buffer, int offset, eltorito_boot_info_t *entry);

#endif /* _ELTORITO_H */
