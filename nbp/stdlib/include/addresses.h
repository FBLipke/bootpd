/**
 * Memory Addresses - BIOS/Real Mode
 * Meaningful names for common memory locations
 */

#ifndef _ADDRESSES_H
#define _ADDRESSES_H

/* ========================================================================
 * BOOT MEMORY
 * ======================================================================== */

/* Default load address for boot files (512KB) */
#define BOOT_LOAD_ADDR       0x10000

/* Traditional boot sector address (15.75KB) */
#define BOOT_SECTOR_ADDR    0x7C00

/* Boot stack address */
#define BOOT_STACK_ADDR     0x7C00

/* ========================================================================
 * HEAP MEMORY
 * ======================================================================== */

/* Heap start (1MB - 64KB) */
#define HEAP_START          0x10000

/* Heap end (1MB - 16KB) */
#define HEAP_END           0xF0000

/* ========================================================================
 * VIDEO MEMORY
 * ======================================================================== */

/* VGA Text Mode memory (color) */
#define VGA_TEXT_COLOR      0xB8000

/* VGA Text Mode memory (mono) */
#define VGA_TEXT_MONO       0xB0000

/* VGA Graphics memory */
#define VGA_GRAPHICS        0xA0000

/* ========================================================================
 * BIOS ROM & PXE
 * ======================================================================== */

/* BIOS ROM area */
#define BIOS_ROM_ADDR       0xF0000
#define BIOS_ROM_SIZE       0x10000

/* PXE ROM signature location */
#define PXE_SIGNATURE_ADDR  0xFF0E0000

/* PXE ROM entry point */
#define PXE_ENTRY_ADDR      0xFF0E0018

/* ========================================================================
 * INT 13 DISK BUFFER
 * ======================================================================== */

/* Default disk buffer (ES for INT 13h) */
#define DISK_BUFFER_ADDR    0x2000

/* ========================================================================
 * SEGMENT ADDRESSES
 * ======================================================================== */

/* Boot segment for loading */
#define SEG_BOOT            0x0000

/* Video segment */
#define SEG_VIDEO           0xB800

/* Memory segment for loading (BOOT_LOAD_ADDR >> 4) */
#define SEG_LOAD            0x1000

/* ========================================================================
 * PORT ADDRESSES (common)
 * ======================================================================== */

/* Keyboard data port */
#define PORT_KBD_DATA       0x60

/* Keyboard status port */
#define PORT_KBD_STATUS     0x64

/* CMOS/RTC port */
#define PORT_CMOS_INDEX     0x70
#define PORT_CMOS_DATA      0x71

/* DMA controller */
#define PORT_DMA_PAGE       0x80

/* ========================================================================
 * BOOT MACROs
 * ======================================================================== */

/* Simple boot jump */
#define BOOT_JUMP(addr) ((void (*)(void))(addr))()

/* Load and jump to BOOT_LOAD_ADDR */
#define BOOT_LOAD_AND_JUMP(size) do { \
    extern uint8_t _boot_data[]; \
    memcpy((void*)BOOT_LOAD_ADDR, _boot_data, size); \
    BOOT_JUMP(BOOT_LOAD_ADDR); \
} while(0)

/* Far jump to boot sector address */
#define BOOT_SECTOR_JUMP() do { \
    __asm__ volatile ("jmp 0x0000:0x7C00"); \
} while(0)

#endif /* _ADDRESSES_H */
