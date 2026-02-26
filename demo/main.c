#include "renderer/platform.h"
#include "renderer/renderer.h"
#include "resources/monkey.h"

#define WIDTH 1980
#define HEIGHT 1080

int main(void)
{
  renderer_context_t *ctx = renderer_create(WIDTH, HEIGHT);
  float array[] = {MONKEY};

  while (!platform_should_close())
  {
    platform_process_events();

    const float move_speed = 0.005f;

    float forward = 0.0f, right = 0.0f, up = 0.0f;
    if (platform_key_down(KEY_W))
      forward += move_speed;
    if (platform_key_down(KEY_S))
      forward -= move_speed;
    if (platform_key_down(KEY_D))
      right += move_speed;
    if (platform_key_down(KEY_A))
      right -= move_speed;
    if (platform_key_down(KEY_E))
      up += move_speed;
    if (platform_key_down(KEY_Q))
      up -= move_speed;

    if (forward != 0.0f || right != 0.0f || up != 0.0f)
      renderer_camera_move_local(ctx, forward, right, up);

    const float rot_speed = 0.003f;

    float yaw_delta = 0.0f, pitch_delta = 0.0f;
    if (platform_key_down(KEY_LEFT))
      yaw_delta -= rot_speed;
    if (platform_key_down(KEY_RIGHT))
      yaw_delta += rot_speed;
    if (platform_key_down(KEY_UP))
      pitch_delta += rot_speed;
    if (platform_key_down(KEY_DOWN))
      pitch_delta -= rot_speed;

    if (yaw_delta != 0.0f || pitch_delta != 0.0f)
      renderer_camera_rotate(ctx, yaw_delta, pitch_delta);

    renderer_begin_frame(ctx);
    renderer_clean(ctx, 0xFF000000);
    renderer_begin(ctx, R_PRIMITIVE_POINT);
    renderer_vertex(ctx, array, MONKEY_FLOATS);
    renderer_end_frame(ctx);
  }

  renderer_destroy(ctx);
  return 0;
}
