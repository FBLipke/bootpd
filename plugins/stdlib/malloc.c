/**
 * malloc / free - Simple heap allocator
 * Uses a simple bump pointer allocator
 */

#include <stdlib.h>

/* Heap area - 64KB starting at 0x10000 */
#define HEAP_START 0x10000
#define HEAP_SIZE  0x10000

static uint8_t *heap_ptr = (uint8_t*)HEAP_START;
static size_t heap_used = 0;

void *malloc(size_t size) {
    /* Align to 4 bytes */
    size = (size + 3) & ~3;
    
    if (heap_used + size > HEAP_SIZE) {
        return NULL;  /* Out of memory */
    }
    
    void *ptr = heap_ptr;
    heap_ptr += size;
    heap_used += size;
    
    return ptr;
}

void *calloc(size_t nmemb, size_t size) {
    size_t total = nmemb * size;
    void *ptr = malloc(total);
    if (ptr) {
        memset(ptr, 0, total);
    }
    return ptr;
}

void *realloc(void *ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    
    if (ptr == NULL) {
        return malloc(size);
    }
    
    /* For simplicity, just allocate new and copy */
    void *new_ptr = malloc(size);
    if (new_ptr) {
        /* Assume old size was the remaining heap */
        memcpy(new_ptr, ptr, size);
    }
    return new_ptr;
}

void free(void *ptr) {
    /* Simple allocator - no actual free */
    (void)ptr;
}

size_t heap_available(void) {
    return HEAP_SIZE - heap_used;
}
