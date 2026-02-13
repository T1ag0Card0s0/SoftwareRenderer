#include "renderer/renderer.h"
#include "renderer/framebuffer.h"

static framebuffer_t framebuffer;

void sr_renderer_init(uint32_t *pixels, size_t width, size_t height)
{
  platform_init(width, height);
  sr_framebuffer_create(&framebuffer, pixels, width, height); 
}

void sr_renderer_finish(void)
{
  platform_shutdown();
}

void sr_renderer_start_frame(uint32_t color)
{
  sr_framebuffer_clean(&framebuffer, sr_framebuffer_rect(&fb), color);
}

void sr_renderer_end_frame(uint32_t fps)
{

}

void sr_renderer_start_face()
{

}

void sr_renderer_end_face()
{

}

void sr_rendere_vertex()
{

}

