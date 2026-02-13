#include "renderer/framebuffer.h"

void sr_framebuffer_create(framebuffer_t* fb, uint32_t* pixels, size_t width, size_t height)
{
  assert(fb);
  assert(pixels);

  fb->pixels = pixels;
  fb->width = width;
  fb->height = height;
}

void sr_framebuffer_clean(framebuffer_t* fb, framebuffer_rect_t rect, uint32_t color)
{
  assert(fb);
  assert(rect.x + rect.width < fb->width);
  assert(rect.y + rect.height < fb->height);

  for(size_t i = rect.y; i < rect.height; i++)
  {
    for(size_t j = rect.x; j < rect.width; j++)
    {
      sr_framebuffer_put_pixel(fb, j, i, color);
    }
  } 
}
