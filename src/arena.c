#include "arena.h"

#include <stdlib.h>
#include <string.h>

static size_t arena_align_up(size_t value, size_t alignment)
{
    return (value + (alignment - 1)) & ~(alignment - 1);
}

arena_t arena_create(size_t buffer_size)
{
    arena_t out = {0};
    if(buffer_size == 0) return out;

    out.buffer = (uint8_t*)calloc(buffer_size, sizeof(uint8_t));
    if (!out.buffer)
    {
        return out;
    }

    out.capacity = buffer_size;
    out.offset = 0;
    return out;
}

void arena_destroy(arena_t *arena)
{
    if (!arena) return;

    free(arena->buffer);
    arena->buffer = NULL;
    arena->capacity = 0;
    arena->offset = 0;
}

void* arena_alloc(arena_t *arena, size_t size)
{
    if (!arena || !arena->buffer || size == 0) 
    {
        return NULL;
    }

    const size_t alignment = sizeof(void*);
    size_t aligned_offset = arena_align_up((size_t)arena->offset, alignment);

    if (aligned_offset > arena->capacity) 
    {
        return NULL;
    }

    if (size > arena->capacity - aligned_offset) 
    {
        return NULL;
    }

    void *ptr = arena->buffer + aligned_offset;
    arena->offset = (arena_mark_t)(aligned_offset + size);

    return ptr;
}

void* arena_calloc(arena_t *arena, size_t size)
{
    arena_mark_t mark = arena->offset;
    void* out = arena_alloc(arena, size);
    memset(&arena->buffer[mark], 0, size);
    return out;
}

arena_mark_t arena_mark(arena_t *arena)
{
    if (!arena) return 0;
    return arena->offset;
}


void arena_unwind(arena_t *arena, arena_mark_t offset)
{
    if (!arena) return;

    if (offset > arena->capacity) 
    {
        offset = arena->capacity;
    }
    arena->offset = offset;
}


