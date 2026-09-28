/**
 * GDT - Global Descriptor Table
 * Memory Segmentation for Protected Mode
 */

#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

/* GDT Entry Size */
#define GDT_ENTRY_SIZE 8
#define GDT_MAX_ENTRIES 16

/* GDT Access Byte flags */
#define GDT_ACCESS_PRESENT    0x80  /* Present bit */
#define GDT_ACCESS_DPL0       0x00  /* Privilege level 0 */
#define GDT_ACCESS_DPL1       0x20  /* Privilege level 1 */
#define GDT_ACCESS_DPL2       0x40  /* Privilege level 2 */
#define GDT_ACCESS_DPL3       0x60  /* Privilege level 3 */
#define GDT_ACCESS_NORMAL      0x92  /* Normal, Read/Write */
#define GDT_ACCESS_CODE       0x9A  /* Code, Execute/Read */
#define GDT_ACCESS_DATA       0x92  /* Data, Read/Write */
#define GDT_ACCESS_TSS        0x89  /* Task State Segment */

/* GDT Granularity flags */
#define GDT_GRANULARITY_4K   0x80
#define GDT_GRANULARITY_32BIT 0x40
#define GDT_GRANULARITY_LIMIT 0x0F

#pragma pack(push, 1)

/* GDT Entry (8 bytes) */
typedef struct {
    uint16_t limit_low;     /* Limit bits 0-15 */
    uint16_t base_low;      /* Base bits 0-15 */
    uint8_t  base_mid;      /* Base bits 16-23 */
    uint8_t  access;        /* Access flags */
    uint8_t  granularity;    /* Granularity + Limit bits 16-19 */
    uint8_t  base_high;     /* Base bits 24-31 */
} gdt_entry_t;

/* GDT Register (48 bits total) */
typedef struct {
    uint16_t limit;         /* GDT limit (size - 1) */
    uint32_t base;          /* GDT base address */
} __attribute__((packed)) gdtr_t;

#pragma pack(pop)

/* Predefined GDT selectors */
#define GDT_SELECTOR_NULL     0x00
#define GDT_SELECTOR_KERNEL_CODE 0x08
#define GDT_SELECTOR_KERNEL_DATA 0x10
#define GDT_SELECTOR_USER_CODE   0x18
#define GDT_SELECTOR_USER_DATA   0x20
#define GDT_SELECTOR_TSS         0x28

/* ========================================================================
 * GDT FUNCTIONS
 * ======================================================================== */

/* Initialize GDT with default entries */
void gdt_init(void);

/* Set a GDT entry */
void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

/* Load GDT - called via assembly */
extern void gdt_load(gdtr_t *gdtr);

/* Get current GDT entry */
void gdt_get_entry(int index, gdt_entry_t *entry);

/* Build flat memory model GDT (4GB, 32-bit) */
void gdt_build_flat_model(void);

/* TSS (Task State Segment) for multitasking */
typedef struct {
    uint16_t prev_task;
    uint16_t reserved1;
    uint32_t esp0;          /* Stack pointer for privilege level 0 */
    uint16_t ss0;           /* Stack segment for privilege level 0 */
    uint16_t reserved2;
    uint32_t esp1;
    uint16_t ss1;
    uint16_t reserved3;
    uint32_t esp2;
    uint16_t ss2;
    uint16_t reserved4;
    uint32_t cr3;           /* Page directory */
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint16_t es;
    uint16_t reserved5;
    uint16_t cs;
    uint16_t reserved6;
    uint16_t ss;
    uint16_t reserved7;
    uint16_t ds;
    uint16_t reserved8;
    uint16_t fs;
    uint16_t reserved9;
    uint16_t gs;
    uint16_t reserved10;
    uint16_t ldt;
    uint16_t reserved11;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed)) tss_t;

/* Initialize TSS */
void tss_init(void);
void tss_update_esp0(uint32_t esp);

#endif /* _GDT_H */
