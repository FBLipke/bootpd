/**
 * printf - Formatted print
 * Supports: %s, %d, %x, %c, %p
 */

#include <stdlib.h>

extern void _print_char(char c);
extern void _print_str(const char *s);

static void put_char(char c) { _print_char(c); }

static void put_str(const char *s) { _print_str(s); }

static void print_int(int val) {
    char buf[12];
    int i = 0;
    int neg = 0;
    
    if (val < 0) {
        neg = 1;
        val = -val;
    }
    if (val == 0) {
        put_char('0');
        return;
    }
    while (val > 0) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }
    if (neg) put_char('-');
    while (i--) put_char(buf[i]);
}

static void print_uint(unsigned int val, int base, int uppercase) {
    char buf[12];
    int i = 0;
    
    if (val == 0) {
        put_char('0');
        return;
    }
    while (val > 0) {
        int digit = val % base;
        if (digit < 10)
            buf[i++] = '0' + digit;
        else
            buf[i++] = (uppercase ? 'A' : 'a') + digit - 10;
        val /= base;
    }
    while (i--) put_char(buf[i]);
}

int printf(const char *fmt, ...) {
    unsigned int *args = (unsigned int*)&fmt + 1;
    int count = 0;
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's': {
                    const char *s = (const char*)*args++;
                    put_str(s ? s : "(null)");
                    break;
                }
                case 'd': {
                    int v = (int)*args++;
                    print_int(v);
                    break;
                }
                case 'u': {
                    unsigned int v = *args++;
                    print_uint(v, 10, 0);
                    break;
                }
                case 'x': {
                    unsigned int v = *args++;
                    put_str("0x");
                    print_uint(v, 16, 0);
                    break;
                }
                case 'X': {
                    unsigned int v = *args++;
                    put_str("0x");
                    print_uint(v, 16, 1);
                    break;
                }
                case 'c': {
                    char c = (char)*args++;
                    put_char(c);
                    break;
                }
                case 'p': {
                    void *p = (void*)*args++;
                    put_str("0x");
                    print_uint((unsigned int)p, 16, 0);
                    break;
                }
                case '%': {
                    put_char('%');
                    break;
                }
                default:
                    put_char('%');
                    put_char(*fmt);
                    break;
            }
        } else {
            put_char(*fmt);
        }
        fmt++;
    }
    return count;
}
