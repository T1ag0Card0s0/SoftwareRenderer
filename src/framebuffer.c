#include "framebuffer.h"

#include <assert.h>

void framebuffer_clean(renderer_context_t *ctx, rect_t rect, uint32_t color)
{
  assert(rect.width * rect.height <= ctx->width * ctx->height);
  assert(rect.x + rect.width <= ctx->width * ctx->height);
  assert(rect.y + rect.height <= ctx->width * ctx->height);

  for (uint32_t i = rect.y; i < rect.y + rect.height; i++)
  {
    for (uint32_t j = rect.x; j < rect.x + rect.width; j++)
    {
      ctx->pixels[(i * ctx->width) + j] = color;
    }
  }
}
