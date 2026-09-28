/**
 * strcpy - Copy a string
 */

#include "include/stdlib.h"

char *strcpy(char *dest, const char *src)
{
    char *d = dest;
    while ((*d++ = *src++))
        ;
    return dest;
}
