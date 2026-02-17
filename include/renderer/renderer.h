#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include <stddef.h>

typedef uint32_t color_t;

typedef enum
{
  RENDERER_NONE = 0,
  RENDERER_POINTS = 1,
  RENDERER_LINES = 2,
  RENDERER_TRIANGLES = 3,
} renderer_primitive_e;

typedef struct
{
  float x;
  float y;
  float z;
} vec3f_t;

#define COLOR_BLACK ((color_t)(0xFF000000))
#define COLOR_GREEN ((color_t)(0xFF00FF00))

void renderer_create(color_t *pixels, size_t width, size_t height);
void renderer_destroy(void);

void renderer_begin_frame(color_t color);
void renderer_end_frame(void);

void renderer_begin(renderer_primitive_e type);
void renderer_end(void);

void renderer_vec_color(color_t color);
void renderer_vec3f(vec3f_t v3f);

#endif // RENDERER_H
