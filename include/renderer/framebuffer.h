#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

typedef struct
{
  uint32_t *pixels;
  size_t width;
  size_t height;
} framebuffer_t;

typedef struct 
{
  size_t x;
  size_t y;
  size_t width;
  size_t height;
} framebuffer_rect_t;

void sr_framebuffer_create(framebuffer_t* fb, uint32_t* pixels, size_t width, size_t height);

void sr_framebuffer_clean(framebuffer_t* fb, framebuffer_rect_t rect, uint32_t color);

#define sr_framebuffer_put_pixel(fb, x, y, color) \
  do \
  { \
    assert(x < fb->width); \
    assert(y < fb->height); \
    fb->pixels[(y * fb->width) + x] = color; \
  } while(0)

#define sr_framebuffer_rect(fb) \
  framebuffer_rect_t{ .x = fb->x, .y = fb->y, .width = fb->width, .height = fb->height }

#endif // FRAMEBUFFER_H
