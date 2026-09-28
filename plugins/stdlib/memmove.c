/**
 * memmove - Copy memory area (handles overlapping)
 */

#include <stdlib.h>

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *d = (uint8_t*)dest;
    const uint8_t *s = (const uint8_t*)src;
    
    if (d < s) {
        /* Copy forward */
        while (n--) *d++ = *s++;
    } else {
        /* Copy backward */
        d += n;
        s += n;
        while (n--) *--d = *--s;
    }
    return dest;
}
