#include "renderer/renderer.h"

#include "algebra.h"
#include "framebuffer.h"
#include "primitive.h"

renderer_context_t renderer_init(pixel_t *pixels, float *depth, uint32_t width, uint32_t height)
{
  return (renderer_context_t){.pixels = pixels, .depth = depth, .width = width, .height = height};
}

void renderer_clean(renderer_context_t *context, color_t color)
{
  framebuffer_clean(context, FRAMEBUFFER_RECT(context->width, context->height), color);
  for (uint32_t i = 0; i < context->width * context->height; i++)
  {
    context->depth[i] = 1.0f;
  }
}

void renderer_begin(renderer_context_t *context, renderer_primitive_e primitive)
{
  context->primitive = primitive;
}

void renderer_end(renderer_context_t *context)
{
  (void)context;
}

void renderer_vertex_color(renderer_context_t *context, color_t color)
{
  context->vertex_color = color;
}

void renderer_vertex_3d(renderer_context_t *context, float *values, size_t values_count)
{
  vec_t vec = (vec_t){.values = values, .size = values_count};
  algebra_rotate_y(vec, context->y_angle);
  algebra_rotate_x(vec, context->x_angle);
  algebra_rotate_z(vec, context->z_angle);
  for (size_t i = 0; i < values_count; i += 3)
  {
    primitive_point(context, values[i], values[i + 1], values[i + 2]);
  }
}

void renderer_rotate_x(renderer_context_t *context, float radians)
{
  context->x_angle = radians;
}

void renderer_rotate_y(renderer_context_t *context, float radians)
{
  context->y_angle = radians;
}

void renderer_rotate_z(renderer_context_t *context, float radians)
{
  context->z_angle = radians;
}

void renderer_scale(renderer_context_t *context, float scale)
{
  context->scale = scale;
}
