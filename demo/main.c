#include "renderer/platform.h"
#include "renderer/renderer.h"
#include "resources/monkey.h"

#include <time.h>
#include <stdio.h>

#define WIDTH 1980
#define HEIGHT 1080

typedef struct 
{
  double fps;
  double fps_accum;
  int fps_frames;

  char fps_text[64];
    struct timespec prev_time;
} fps_t;

void update_fps(fps_t *fps_data)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    double dt = (now.tv_sec - fps_data->prev_time.tv_sec) +
                (now.tv_nsec - fps_data->prev_time.tv_nsec) / 1000000000.0;

    fps_data->prev_time = now;

    fps_data->fps_accum += dt;
    fps_data->fps_frames++;
    if (fps_data->fps_accum >= 0.5)
    {
      fps_data->fps = (double)fps_data->fps_frames / fps_data->fps_accum;
      fps_data->fps_frames = 0;
      fps_data->fps_accum = 0.0;
    }
    snprintf(fps_data->fps_text, sizeof(fps_data->fps_text), "FPS: %.1f", fps_data->fps);
}

int main(void)
{
  renderer_context_t *ctx = renderer_create(WIDTH, HEIGHT);
  float array[] = {MONKEY};
renderer_text_style_t hud = {
    .size = 2,
    .fg = 0xFFFFFFFF,
    .bg = 0x80000000,
    .draw_bg = 1
};
  fps_t fps_data = {0};

  clock_gettime(CLOCK_MONOTONIC, &fps_data.prev_time);
  
    while (!platform_should_close())
  {
    platform_process_events();
    update_fps(&fps_data); 
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
      yaw_delta += rot_speed;
    if (platform_key_down(KEY_RIGHT))
      yaw_delta -= rot_speed;
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

    renderer_draw_text(ctx, 10, 10, "W forward\nA left\nS backwards\nD right\nQ up\nE down\nArrows to rotate", &hud);
    renderer_draw_text(ctx, 10, 200, fps_data.fps_text, &hud);

    renderer_end_frame(ctx);
  }

  renderer_destroy(ctx);
  return 0;
}
