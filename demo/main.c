#include "monkey.h"
#include "renderer/platform.h"
#include "renderer/renderer.h"

#define WIDTH 1920
#define HEIGHT 1080

static pixel_t pixels[WIDTH * HEIGHT];
static float depth[WIDTH * HEIGHT];

int main(void)
{
  renderer_context_t ctx = renderer_init(pixels, depth, WIDTH, HEIGHT);
  platform_init(WIDTH, HEIGHT);

  float vertices[] = {MONKEY};
  renderer_scale(&ctx, 0.5f);
  while (1)
  {
    renderer_clean(&ctx, COLOR_ARGB(0x00, 0x00, 0x00, 0x00));

    renderer_begin(&ctx, RENDERER_PRIMITIVE_POINT);
    renderer_rotate_y(&ctx, 0.01f);
    renderer_rotate_x(&ctx, 0.01f);
    renderer_vertex_color(&ctx, COLOR_ARGB(0xFF, 0x00, 0xFF, 0x00));
    renderer_vertex_3d(&ctx, vertices, MONKEY_FLOATS);
    renderer_draw_text(&ctx, 0, 0, "Hello World", COLOR_ARGB(0xFF, 0x00, 0x00, 0xFF));
    renderer_end(&ctx);
    platform_present(pixels, WIDTH, HEIGHT);
  }

  return 0;
}
