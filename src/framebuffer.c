#include "framebuffer.h"

#include <assert.h>

void framebuffer_clean(pixel_t *pixels, uint32_t size, rect_t rect, uint32_t color)
{
  if(rect.width * rect.height > size) return;
  if(rect.x + rect.width > size) return;
  if(rect.y + rect.height > size) return;

  for(uint32_t i = rect.y; i < rect.y + rect.height; i++)
  {
    for(uint32_t j = rect.x; j < rect.x + rect.width; j++)
    {
      *(pixels + ((i * rect.width) + j)) = color;
    }
  }
}


