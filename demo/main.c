#include "renderer/renderer.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define W 640
#define H 480

int main(void)
{
  color_t pixels[W * H] = {0};

  renderer_create(pixels, W, H);

  vec3f_t base_cube[8] = {{1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, -1.0f},
                          {1.0f, 1.0f, -1.0f},  {1.0f, 1.0f, 1.0f},  {-1.0f, 1.0f, 1.0f},  {-1.0f, 1.0f, -1.0f}};
  vec3f_t cube[8] = {0};

  int indices[] = {0, 1, 2, 2, 3, 0, 4, 6, 5, 4, 7, 6, 3, 2, 6, 6, 7, 3,
                   0, 5, 1, 0, 4, 5, 0, 3, 7, 7, 4, 0, 1, 5, 6, 6, 2, 1};
  size_t index_count = sizeof(indices) / sizeof(indices[0]);

  float angle = 0.0f;
  float delta = 0.01f;

  while (1)
  {
    angle += delta;

    memcpy(cube, base_cube, sizeof(cube));
    renderer_rotateX(cube, 8, angle);
    renderer_rotateY(cube, 8, angle);
    renderer_rotateZ(cube, 8, angle);

    renderer_begin_frame(COLOR_RGB(0, 0, 0));
    renderer_begin(RENDERER_POINTS);
    for (size_t i = 0; i < index_count; i += 3)
    {
      renderer_vec_color(COLOR_RGB(255, 0, 0));
      renderer_vec3f(cube[indices[i]]);
      renderer_vec_color(COLOR_RGB(0, 255, 0));
      renderer_vec3f(cube[indices[i + 1]]);
      renderer_vec_color(COLOR_RGB(0, 0, 255));
      renderer_vec3f(cube[indices[i + 2]]);
    }
    renderer_end();
    renderer_end_frame();
    usleep(16667);
  }

  renderer_destroy();
  return 0;
}
