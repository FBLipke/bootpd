/*
 * NBP - Network Bootstrap Program
 * Minimal PXE Bootable Executable
 */

.code16gcc

.section .text
.global _start

_start:
    # Save boot parameters (passed by PXE ROM)
    pushw %cs
    popw %ds
    
    # Print "NBP" using BIOS interrupt
    movw $0x0001, %ax
    int $0x10
    
    # Infinite loop (for now)
1:
    jmp 1b

