#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>
#include <stddef.h>

typedef size_t arena_mark_t;

typedef struct 
{
    uint8_t *buffer;
    size_t capacity;
    arena_mark_t offset; 
} arena_t;

arena_t arena_create(size_t buffer_size);
void arena_destroy(arena_t *arena);

void* arena_alloc(arena_t *arena, size_t size);
void* arena_calloc(arena_t *arena, size_t size);

arena_mark_t arena_mark(arena_t *arena);
void arena_unwind(arena_t *arena, size_t offset);

#endif // ARENA_H
