#ifndef RENDERER_H
#define RENDERER_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t pixel_t;

typedef struct renderer_context renderer_context_t;

typedef enum
{
    R_PRIMITIVE_POINT,
    R_PRIMITIVE_WIRE_FRAME
} renderer_primitives_e;

typedef struct renderer_text_style_t
{
    int size;
    pixel_t fg;
    pixel_t bg;
    int draw_bg;
} renderer_text_style_t;

renderer_context_t *renderer_create(size_t width, size_t height);

void renderer_destroy(renderer_context_t *context);

void renderer_begin_frame(renderer_context_t* context);

void renderer_end_frame(renderer_context_t* context);

void renderer_clean(renderer_context_t* context, pixel_t color);

void renderer_begin(renderer_context_t *context, renderer_primitives_e primitive);

void renderer_vertex(renderer_context_t* context, float *vertices, size_t size);

void renderer_camera_move_local(renderer_context_t* ctx, float forward, float right, float up);

void renderer_camera_rotate(renderer_context_t* ctx, float yaw_delta, float pitch_delta);

void renderer_draw_text(renderer_context_t *ctx, int x, int y, const char *text, const renderer_text_style_t *style);

void renderer_draw_indexed(renderer_context_t *ctx,
                           const float *vertices, size_t vertex_floats,
                           const uint32_t *indices, size_t index_count);

#endif // RENDERER_H
