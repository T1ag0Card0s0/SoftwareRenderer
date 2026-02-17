#include <stdint.h>

#include "renderer/renderer.h"

#define W 640
#define H 480

int main(void)
{
  color_t pixels[W * H] = {0};

  renderer_create(pixels, W, H);

  while (1)
  {
    renderer_begin_frame(COLOR_BLACK);
    renderer_begin(RENDERER_POINTS);
    renderer_vec_color(COLOR_GREEN);
    renderer_vec3f((vec3f_t){.x = 0.0f, .y = 0.0f, .z = 0.0f});
    renderer_end();
    renderer_end_frame();
  }

  renderer_destroy();
  return 0;
}
