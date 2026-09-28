/**
 * print_str - Print a string via BIOS
 */

#include <stdlib.h>

extern void _print_str(const char *s);

void print_str(const char *s) { _print_str(s); }

void print_newline(void) { _print_str("\r\n"); }

/* NBP compatibility wrappers */
void _print_char(char c) { print_char(c); }
void _print_str(const char *s) { print_str(s); }
void _print_newline(void) { print_newline(); }
