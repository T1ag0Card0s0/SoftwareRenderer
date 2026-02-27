#include "primitive.h"

#include <stdlib.h>

#define X_INDEX 0
#define Y_INDEX 1

void primitive_point(framebuffer_t *framebuffer, float vertex[3], pixel_t color)
{
  int x = (int)(((vertex[X_INDEX] + 1.0f) * 0.5f) * (framebuffer->width - 1));
  int y = (int)((1.0f - ((vertex[Y_INDEX] + 1.0f) * 0.5f)) * (framebuffer->height - 1));

  if (x < 0 || x >= framebuffer->width || y < 0 || y >= framebuffer->height)
    return;

  framebuffer->pixels[y * framebuffer->width + x] = color;
}

static void draw_line(framebuffer_t *fb, int x0, int y0, int x1, int y1, pixel_t color)
{
  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;

  while (1)
  {
    if (x0 >= 0 && x0 < fb->width && y0 >= 0 && y0 < fb->height)
      fb->pixels[y0 * fb->width + x0] = color;

    if (x0 == x1 && y0 == y1)
      break;

    int e2 = 2 * err;

    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }
  }
}

void primitive_wireframe(framebuffer_t *framebuffer, float vertex[9], pixel_t color)
{
  int x[3], y[3];

  for (int i = 0; i < 3; ++i)
  {
    float ndc_x = vertex[i * 3 + 0];
    float ndc_y = vertex[i * 3 + 1];

    x[i] = (int)(((ndc_x + 1.0f) * 0.5f) * (framebuffer->width - 1));
    y[i] = (int)((1.0f - ((ndc_y + 1.0f) * 0.5f)) * (framebuffer->height - 1));
  }

  draw_line(framebuffer, x[0], y[0], x[1], y[1], color);
  draw_line(framebuffer, x[1], y[1], x[2], y[2], color);
  draw_line(framebuffer, x[2], y[2], x[0], y[0], color);
}
