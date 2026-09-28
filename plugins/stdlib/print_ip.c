/**
 * print_ip - Print an IPv4 address
 */

#include <stdlib.h>

extern void _print_char(char c);

void print_ip(uint32_t ip) {
    print_hex8((uint8_t)(ip >> 24));
    _print_char('.');
    print_hex8((uint8_t)(ip >> 16));
    _print_char('.');
    print_hex8((uint8_t)(ip >> 8));
    _print_char('.');
    print_hex8((uint8_t)(ip & 0xFF));
}
