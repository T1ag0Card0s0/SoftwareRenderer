#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "renderer/renderer.h"

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

typedef struct 
{
  uint32_t x;
  uint32_t y;
  uint32_t width;
  uint32_t height;
} rect_t;

void framebuffer_clean(renderer_context_t *ctx, rect_t rect, uint32_t color);

#define FRAMEBUFFER_RECT(w, h) \
  (rect_t){ .x = 0, .y = 0, .width = w, .height = h }

#endif // FRAMEBUFFER_H
