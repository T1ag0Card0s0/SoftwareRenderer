#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "framebuffer.h"
#include "renderer/renderer.h"

typedef struct
{
  vec3f_t vec;
  color_t color;
} vertex_t;

void primitive_point(framebuffer_t *fb, vertex_t vertex);

void primitive_line(framebuffer_t *fb, vertex_t vertices[2]);

void primitive_triangle(framebuffer_t *fb, vertex_t vertices[3]);

#endif // PRIMITIVES_H
