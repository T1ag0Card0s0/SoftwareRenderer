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

#define COLOR_R(c) (((c) >> 16) & 0xFF)
#define COLOR_G(c) (((c) >> 8) & 0xFF)
#define COLOR_B(c) (((c)) & 0xFF)
#define COLOR_RGB(r,g,b) (((r) << 16) | ((g) << 8) | (b))

void renderer_create(color_t *pixels, size_t width, size_t height);
void renderer_destroy(void);

void renderer_begin_frame(color_t color);
void renderer_end_frame(void);

void renderer_begin(renderer_primitive_e type);
void renderer_end(void);

void renderer_vec_color(color_t color);
void renderer_vec3f(vec3f_t v3f);

void renderer_rotateX(vec3f_t *v, size_t n, float angle);
void renderer_rotateY(vec3f_t *v, size_t n, float angle);
void renderer_rotateZ(vec3f_t *v, size_t n, float angle);
void renderer_scale(vec3f_t *v, size_t n, float scale, vec3f_t center);

#endif // RENDERER_H
