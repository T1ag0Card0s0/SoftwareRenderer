#include "renderer/renderer.h"

renderer_context_t renderer_init(pixel_t *pixels, uint32_t width, uint32_t height)
{
  return (renderer_context_t) 
  {
      .pixels = pixels,
      .width = width,
      .height = height
  };
}
