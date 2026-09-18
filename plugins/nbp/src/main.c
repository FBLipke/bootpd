/*
 * NBP - Network Bootstrap Program
 * Main entry point for PXE boot
 */

#include <stdint.h>
#include <stddef.h>

/* Video functions */
void clear_screen(void);
void print_str(const char* str);
void draw_progressbar(uint32_t current, uint32_t total);

/* Network functions - to be implemented */
int network_init(void);
int dhcp_discover(void);
int tftp_download(const char* filename, void* buffer, size_t size);

/* Boot menu */
void show_boot_menu(void);

/* Progress bar - ASCII art style */
void draw_progressbar(uint32_t current, uint32_t total) {
    const char* leading = "░";
    const char* progress = "▓";
    const int width = 40;
    
    float ratio = (float)current / total;
    int filled = (int)(ratio * width);
    int percent = (int)(ratio * 100);
    
    print_str("\r BootpdClient [");
    
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            print_str(progress);
        } else {
            print_str(leading);
        }
    }
    
    print_str("] ");
    
    /* Print percentage - simplified */
    print_str("0%");
}

void clear_screen(void) {
    /* BIOS scroll up - AH=6, AL=0 (clear), BH=7 (attribute), CX=0, DX=0x184F */
    __asm__ volatile (
        "mov $0x0600, %%ax\n\t"
        "mov $0x07, %%bh\n\t"
        "xor %%cx, %%cx\n\t"
        "mov $0x184F, %%dx\n\t"
        "int $0x10\n\t"
        : : : "ax", "bx", "cx", "dx"
    );
}

void print_str(const char* str) {
    while (*str) {
        __asm__ volatile (
            "mov $0x0E00, %%ax\n\t"
            "int $0x10\n\t"
            : : "a"(*str), "b"(0x0007) : "ax"
        );
        str++;
    }
}

int main(void) {
    /* Clear screen */
    clear_screen();
    
    /* Print welcome */
    print_str("\r\n");
    print_str("========================================\r\n");
    print_str("       BootpdClient NBP v0.1          \r\n");
    print_str("========================================\r\n");
    print_str("\r\n");
    
    /* Show progress bar demo */
    for (int i = 0; i <= 100; i += 10) {
        draw_progressbar(i, 100);
    }
    
    print_str("\r\n");
    print_str("Initializing network...\r\n");
    
    /* Network init would go here */
    network_init();
    
    /* Show boot menu */
    show_boot_menu();
    
    return 0;
}

/* Stubs for future implementation */
int network_init(void) {
    print_str("Network not yet implemented\r\n");
    return 0;
}

int dhcp_discover(void) {
    return 0;
}

int tftp_download(const char* filename, void* buffer, size_t size) {
    return 0;
}

void show_boot_menu(void) {
    print_str("\r\n");
    print_str("Boot Menu:\r\n");
    print_str("  1. Windows PE\r\n");
    print_str("  2. Linux Rescue\r\n");
    print_str("  3. Diagnostics\r\n");
    print_str("\r\n");
    print_str("Select option: ");
}
