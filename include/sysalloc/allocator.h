#ifndef _RE_ALLOCATOR_H
#define _RE_ALLOCATOR_H

#include "../attributes.h"
#include <stddef.h>

static char   heap[1000 * 1024]; // 1 MB
static size_t heap_bumper = 0;

// god is dead and i killed him

extern void* malloc(size_t size);

extern void* calloc(size_t groups, size_t group_size);

/**
 * Get an aligned block of memory
 * The memory must be freed by aligned_free
 * WARNING: Only supports power of two alignments
 * https://tabreztalks.medium.com/memory-aligned-malloc-6c7b562d58d0
 */
extern void* aligned_alloc(size_t sz, size_t alignment);

extern void* aligned_realloc(void* orig, size_t size, size_t alignment);

/**
 * Get the actual pointer allocated by aligned_alloc()\*realloc()
 * WARNING: If the block had been freed(), this function returns NULL
 */
extern void* aligned_get_absolute_ptr(void* aligned_ptr);

/**
 * Get the size allocated by aligned_alloc()\*realloc
 */
extern size_t aligned_ptr_get_size(void* aligned_ptr);

extern void* realloc(void* prevblock, size_t new_sz);

extern void free(void* block);

extern void aligned_free(void* aligned_block);

#endif //_RE_ALLOCATOR_H
