#include <stdint.h>

#include "renderer/renderer.h"

#define W 640
#define H 480

int main()
{
  uint32_t pixels[W * H] = {0};
  sr_renderer_init(pixels, W, H);

  int frame = 0;

  while (platform_process_events())
  {
    renderer_draw(framebuffer, W, H, frame++);
    platform_present(framebuffer);
  }

  platform_shutdown();
  return 0;
}
