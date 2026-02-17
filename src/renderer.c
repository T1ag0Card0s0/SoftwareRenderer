#include "renderer/renderer.h"
#include "renderer/platform.h"
#include "renderer/config.h"

#include "framebuffer.h"

static framebuffer_t framebuffer;

void renderer_create(color_t *pixels, size_t width, size_t height)
{
  platform_init(width, height);
  framebuffer_create(&framebuffer, pixels, width, height); 
}

void renderer_destroy(void)
{
  platform_shutdown();
}

void renderer_clean(color_t color)
{
  framebuffer_clean(&framebuffer, framebuffer_rect(&framebuffer), color);
}

void renderer_begin_frame(void)
{

}

void renderer_end_frame(void)
{

}

void renderer_begin(renderer_primitive_e type)
{

}

void renderer_end(void)
{

}

void renderer_vec_color(color_t color)
{

}

void renderer_vec3f(vec3f_t v3f)
{

}


