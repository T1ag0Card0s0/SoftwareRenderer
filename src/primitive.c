#include "primitive.h"

#include <stdlib.h>

#define X_INDEX 0
#define Y_INDEX 1

void primitive_point(framebuffer_t *framebuffer, float vertex[3], pixel_t color)
{
  int x = (int)(((vertex[X_INDEX] + 1.0f) * 0.5f) * (framebuffer->width - 1));
  int y = (int)((1.0f - ((vertex[Y_INDEX] + 1.0f) * 0.5f)) * (framebuffer->height - 1));

  if (x < 0 || x >= framebuffer->width || y < 0 || y >= framebuffer->height)
  {
    return;
  }

  framebuffer->pixels[y * framebuffer->width + x] = color;
}

void primitive_line(framebuffer_t *framebuffer, float vertex[6], pixel_t color)
{
  int x0 = (int)(((vertex[0] + 1.0f) * 0.5f) * (framebuffer->width - 1));
  int y0 = (int)((1.0f - ((vertex[1] + 1.0f) * 0.5f)) * (framebuffer->height - 1));
  int x1 = (int)(((vertex[3] + 1.0f) * 0.5f) * (framebuffer->width - 1));
  int y1 = (int)((1.0f - ((vertex[4] + 1.0f) * 0.5f)) * (framebuffer->height - 1));

  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;

  while (1)
  {
    if (x0 >= 0 && x0 < framebuffer->width && y0 >= 0 && y0 < framebuffer->height)
    {
      framebuffer->pixels[y0 * framebuffer->width + x0] = color;
    }

    if (x0 == x1 && y0 == y1)
    {
      break;
    }

    int e2 = 2 * err;
    if (e2 >= dy)
    {
      err += dy;
      x0 += sx;
    }

    if (e2 <= dx)
    {
      err += dx;
      y0 += sy;
    }
  }
}
