#ifndef RENDERER_H
#define RENDERER_H

#include "algebra.h"

#include <stdint.h>
#include <stddef.h>

typedef uint32_t pixel_t;

typedef struct 
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;
} color_t;

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
  uint32_t width;
  uint32_t height;
  renderer_primitive_e primitive;
  color_t vertex_color;
} renderer_context_t;

typedef struct
{
  vec3_t coordinates;
  color_t color;
} vertex_t;

renderer_context_t renderer_init(pixel_t *pixels, uint32_t width, uint32_t height);

void renderer_clean(renderer_context_t* context, color_t color);

void renderer_begin(renderer_context_t *context, renderer_primitive_e primitive);
void renderer_end(renderer_context_t *context);

void renderer_vertex_color(renderer_context_t *context, color_t color);
void renderer_vertex(renderer_context_t *context, vec3_t coordinates);

void platform_init(int width, int height);
int platform_process_events(void);
void platform_present(pixel_t *pixels, size_t width, size_t height);
void platform_shutdown(void);

#endif // RENDERER_H
