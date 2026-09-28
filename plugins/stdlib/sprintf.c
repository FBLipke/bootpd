/**
 * sprintf - Format string to buffer
 * Supports: %s, %d, %u, %x, %X, %c, %p, %%
 */

#include "include/stdlib.h"

static void write_char(char **buf, char c)
{
    *(*buf)++ = c;
}

static void write_str(char **buf, const char *s)
{
    while (*s)
        *(*buf)++ = *s++;
}

static void write_int(char **buf, int val)
{
    char tmp[12];
    int i = 0;
    int neg = 0;

    if (val < 0)
    {
        neg = 1;
        val = -val;
    }
    if (val == 0)
    {
        write_char(buf, '0');
        return;
    }
    while (val > 0)
    {
        tmp[i++] = '0' + (val % 10);
        val /= 10;
    }
    if (neg)
        write_char(buf, '-');
    while (i--)
        write_char(buf, tmp[i]);
}

static void write_uint(char **buf, unsigned int val, int base, int uppercase)
{
    char tmp[12];
    int i = 0;

    if (val == 0)
    {
        write_char(buf, '0');
        return;
    }
    while (val > 0)
    {
        int digit = val % base;
        if (digit < 10)
            tmp[i++] = '0' + digit;
        else
            tmp[i++] = (uppercase ? 'A' : 'a') + digit - 10;
        val /= base;
    }
    while (i--)
        write_char(buf, tmp[i]);
}

int sprintf(char *buf, const char *fmt, ...)
{
    unsigned int *args = (unsigned int *)&fmt + 1;

    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++;
            switch (*fmt)
            {
            case 's':
            {
                const char *s = (const char *)*args++;
                write_str(&buf, s ? s : "(null)");
                break;
            }
            case 'd':
            {
                int v = (int)*args++;
                write_int(&buf, v);
                break;
            }
            case 'u':
            {
                unsigned int v = *args++;
                write_uint(&buf, v, 10, 0);
                break;
            }
            case 'x':
            {
                unsigned int v = *args++;
                write_str(&buf, "0x");
                write_uint(&buf, v, 16, 0);
                break;
            }
            case 'X':
            {
                unsigned int v = *args++;
                write_str(&buf, "0x");
                write_uint(&buf, v, 16, 1);
                break;
            }
            case 'c':
            {
                char c = (char)*args++;
                write_char(&buf, c);
                break;
            }
            case 'p':
            {
                void *p = (void *)*args++;
                write_str(&buf, "0x");
                write_uint(&buf, (unsigned int)p, 16, 0);
                break;
            }
            case '%':
            {
                write_char(&buf, '%');
                break;
            }
            default:
                write_char(&buf, '%');
                write_char(&buf, *fmt);
                break;
            }
        }
        else
        {
            write_char(&buf, *fmt);
        }
        fmt++;
    }
    *buf = '\0';
    return 0;
}
