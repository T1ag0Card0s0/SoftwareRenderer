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

void framebuffer_create(framebuffer_t* fb, uint32_t* pixels, size_t width, size_t height);

void framebuffer_clean(framebuffer_t* fb, framebuffer_rect_t rect, uint32_t color);

static inline void framebuffer_put_pixel(framebuffer_t *fb, size_t x, size_t y, uint32_t color)
{
    assert(x < fb->width);
    assert(y < fb->height);
    fb->pixels[(y * fb->width) + x] = color;

}

static inline framebuffer_rect_t framebuffer_rect(framebuffer_t *fb)
{
  return (framebuffer_rect_t){ .x = 0, .y = 0, .width = fb->width, .height = fb->height };
}

#endif // FRAMEBUFFER_H
