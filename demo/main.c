#include <stdint.h>

#include "renderer/renderer.h"

#define W 640
#define H 480

int main(void)
{
  color_t pixels[W * H] = {0};
    
  renderer_ctx_t* rnd = renderer_create(pixels, W, H);

  while(1)
  {
    renderer_clean(rnd, COLOR_BLACK);

    renderer_begin(rnd, RENDERER_POINTS);

    renderer_vec_color(rnd, COLOR_GREEN);

    renderer_vec3f(rnd, (vec3f_t){.x = 0.0f, .y = 0.0f, .z = 0.0f});

    renderer_end(rnd);
  }

  renderer_destroy(rnd);
  return 0;
}
