/**
 * memcpy - Copy memory area
 */

#include <stdlib.h>

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = (uint8_t*)dest;
    const uint8_t *s = (const uint8_t*)src;
    while (n--) *d++ = *s++;
    return dest;
}
