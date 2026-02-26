#ifndef RENDERER_H
#define RENDERER_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t pixel_t;

typedef struct renderer_context renderer_context_t;

renderer_context_t *renderer_create(size_t width, size_t height);

void renderer_destroy(renderer_context_t *ctx);


#endif // RENDERER_H
