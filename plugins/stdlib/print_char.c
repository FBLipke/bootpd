/**
 * print_char - Print a single character via BIOS
 */

#include <stdlib.h>

extern void _print_char(char c);

void print_char(char c) { _print_char(c); }
