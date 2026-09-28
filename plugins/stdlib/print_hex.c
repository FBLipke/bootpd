/**
 * print_hex - Print values in hexadecimal
 */

#include <stdlib.h>

extern void _print_char(char c);

void print_hex8(uint8_t val) {
    uint8_t h1 = (val >> 4) & 0x0F;
    uint8_t h2 = val & 0x0F;
    _print_char(h1 < 10 ? ('0' + h1) : ('A' + h1 - 10));
    _print_char(h2 < 10 ? ('0' + h2) : ('A' + h2 - 10));
}

void print_hex16(uint16_t val) {
    print_hex8((uint8_t)(val >> 8));
    print_hex8((uint8_t)(val & 0xFF));
}

void print_hex32(uint32_t val) {
    print_hex16((uint16_t)(val >> 16));
    print_hex16((uint16_t)(val & 0xFFFF));
}
