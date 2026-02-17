#include "renderer/renderer.h"
#include "framebuffer.h"
#include "primitives.h"
#include "renderer/config.h"
#include "renderer/platform.h"

#include <string.h>

typedef struct
{
  framebuffer_t framebuffer;
  renderer_primitive_e primitive;
  color_t color;
  vertex_t vertices[MAX_PRIMITIVE_VERTEX_NUMBER];
  uint8_t curr_vecs_num;
} renderer_context_t;

static renderer_context_t renderer_context;

void renderer_create(color_t *pixels, size_t width, size_t height)
{
  platform_init(width, height);
  framebuffer_create(&renderer_context.framebuffer, pixels, width, height);
}

void renderer_destroy(void)
{
  platform_shutdown();
}

void renderer_begin_frame(color_t color)
{
  platform_process_events();
  framebuffer_clean(&renderer_context.framebuffer, framebuffer_rect(&renderer_context.framebuffer), color);
}

void renderer_end_frame(void)
{
  platform_present(renderer_context.framebuffer.pixels, renderer_context.framebuffer.width,
                   renderer_context.framebuffer.height);
}

void renderer_begin(renderer_primitive_e type)
{
  renderer_context.primitive = type;
}

void renderer_end(void)
{
  renderer_context.primitive = RENDERER_NONE;
  renderer_context.curr_vecs_num = 0;
}

void renderer_vec_color(color_t color)
{
  renderer_context.color = color;
}

void renderer_vec3f(vec3f_t v3f)
{
  if (renderer_context.primitive == RENDERER_NONE)
    return;

  vertex_t vertex = {.vec = v3f, .color = renderer_context.color};
  if (renderer_context.primitive == RENDERER_POINTS)
  {
    primitive_point(&renderer_context.framebuffer, vertex);
  }
  else
  {
    memcpy(&renderer_context.vertices[renderer_context.curr_vecs_num++], &vertex, sizeof(vertex));
    if (renderer_context.primitive == RENDERER_LINES && renderer_context.curr_vecs_num == 2)
    {
      primitive_line(&renderer_context.framebuffer, renderer_context.vertices);
      renderer_context.curr_vecs_num = 0;
    }
    else if (renderer_context.primitive == RENDERER_TRIANGLES && renderer_context.curr_vecs_num == 3)
    {
      primitive_triangle(&renderer_context.framebuffer, renderer_context.vertices);
      renderer_context.curr_vecs_num = 0;
    }
  }
}
