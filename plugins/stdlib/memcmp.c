/**
 * memcmp - Compare two memory areas
 */

#include "include/stdlib.h"

void *memcmp(const void *s1, const void *s2, size_t n)
{
    const uint8_t *a = (const uint8_t *)s1;
    const uint8_t *b = (const uint8_t *)s2;
    while (n--)
    {
        if (*a != *b)
            return (void *)(*a - *b);
        a++;
        b++;
    }
    return 0;
}
