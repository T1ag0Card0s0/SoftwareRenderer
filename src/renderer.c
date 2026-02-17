#include "renderer/renderer.h"
#include "framebuffer.h"
#include "renderer/platform.h"
#include "vertex.h"

typedef struct
{
  framebuffer_t framebuffer;
  renderer_primitive_e primitive;
  color_t color;
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
}

void renderer_vec_color(color_t color)
{
  renderer_context.color = color;
}

void renderer_vec3f(vec3f_t v3f)
{
  vec3f_put_pixel(&renderer_context.framebuffer, v3f, renderer_context.color);
}
