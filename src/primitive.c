#include "primitive.h"

#define POINT_DIMENTION 10

void primitive_point(renderer_context_t *ctx, float x, float y, float z)
{
  float perspective = 1.0f / (z + 3.0f);
  float fx = ((float)ctx->width / 2.0f) + x * ((float)ctx->width) * perspective;
  float fy = ((float)ctx->height / 2.0f) - y * ((float)ctx->height) * perspective;
  uint32_t screen_x = (uint32_t)(fx + 0.5f);
  uint32_t screen_y = (uint32_t)(fy + 0.5f);

  //    uint32_t screen_x = (int)((x + 1.0f) * 0.5f * ctx->width);
  //    uint32_t screen_y = (int)((1.0f - y) * 0.5f * ctx->height);

  float depth_value = (z + 1.0f) * 0.5f;

  if (screen_x >= ctx->width || screen_y >= ctx->height)
    return;

  int index = screen_y * ctx->width + screen_x;

  if (depth_value < ctx->depth[index])
  {
    ctx->depth[index] = depth_value;
    ctx->pixels[index] = ctx->vertex_color;
  }
}
