#include "vertex.h"
#include "renderer/config.h"

void vec3f_put_pixel(framebuffer_t *fb, vec3f_t v, color_t color)
{
  int cx = (int)((v.x * 0.5f + 0.5f) * (float)fb->width);
  int cy = (int)((-v.y * 0.5f + 0.5f) * (float)fb->height);

  int r = POINT_RADIUS;
  int r2 = r * r;

  for (int dy = -r; dy <= r; dy++)
  {
    for (int dx = -r; dx <= r; dx++)
    {
      if (dx * dx + dy * dy <= r2)
      {
        int px = cx + dx;
        int py = cy + dy;

        if (px >= 0 && px < (int)fb->width && py >= 0 && py < (int)fb->height)
          fb->pixels[py * fb->width + px] = color;
      }
    }
  }
}
