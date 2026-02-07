#include "renderer/renderer.h"

void renderer_draw(uint32_t *fb, int w, int h, int frame)
{
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
    {
      fb[y * w + x] = 0xff000000 | (((x + frame) ^ (y + frame)) & 255);
    }
}
