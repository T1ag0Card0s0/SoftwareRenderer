#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include <stddef.h>

typedef uint32_t pixel_t;
typedef uint32_t color_t;

#define COLOR_ARGB(alpha, red, green, blue) \
    ((alpha & 0xFFU) << 24) |  \
    ((red & 0xFFU) << 16) | \
    ((green & 0xFFU) << 8) | \
    (blue & 0xFFU)

typedef enum 
{
  RENDERER_PRIMITIVE_NONE = 0,
  RENDERER_PRIMITIVE_POINT = 1,
  RENDERER_PRIMITIVE_LINE = 2,
  RENDERER_PRIMITIVE_TRIANGLE = 3
} renderer_primitive_e;

typedef struct 
{
  pixel_t *pixels;
  float *depth;
  uint32_t width;
  uint32_t height;
  renderer_primitive_e primitive;
  color_t vertex_color;
  float x_angle, y_angle, z_angle;
  float scale;
} renderer_context_t;

renderer_context_t renderer_init(pixel_t *pixels, float *depth, uint32_t width, uint32_t height);

void renderer_clean(renderer_context_t* context, color_t color);

void renderer_begin(renderer_context_t *context, renderer_primitive_e primitive);
void renderer_end(renderer_context_t *context);

void renderer_vertex_color(renderer_context_t *context, color_t color);
void renderer_vertex_3d(renderer_context_t *context, float *values, size_t values_count);

void renderer_draw_text(renderer_context_t *context, float x, float y, const char *text, color_t color);

void renderer_rotate_x(renderer_context_t *context, float radians);
void renderer_rotate_y(renderer_context_t *context, float radians);
void renderer_rotate_z(renderer_context_t *context, float radians);
void renderer_scale(renderer_context_t *context, float scale);
#endif // RENDERER_H
