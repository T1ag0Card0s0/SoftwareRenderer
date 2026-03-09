#include "renderer/platform.h"
#include "renderer/renderer.h"
#include "resources/monkey.h"
#include "ui_menu.h"

#include <stdio.h>
#include <time.h>

#define WIDTH 1920
#define HEIGHT 1080

typedef struct
{
  int draw_help;
  int draw_fps;
  int wireframe_mode;
  int paused;
} app_state_t;

static void menu_toggle_help(void *user_data)
{
  app_state_t *app = (app_state_t *)user_data;
  app->draw_help = !app->draw_help;
}

static void menu_toggle_fps(void *user_data)
{
  app_state_t *app = (app_state_t *)user_data;
  app->draw_fps = !app->draw_fps;
}

static void menu_toggle_wireframe(void *user_data)
{
  app_state_t *app = (app_state_t *)user_data;
  app->wireframe_mode = !app->wireframe_mode;
}

static void menu_toggle_pause(void *user_data)
{
  app_state_t *app = (app_state_t *)user_data;
  app->paused = !app->paused;
}

typedef struct
{
  double fps;
  double fps_instant;
  double dt;
  char fps_text[64];
  struct timespec prev_time;
} fps_t;

static double clampd(double v, double lo, double hi)
{
  return v < lo ? lo : (v > hi ? hi : v);
}

void update_fps(fps_t *fps_data)
{
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);

  double dt = (now.tv_sec - fps_data->prev_time.tv_sec) + (now.tv_nsec - fps_data->prev_time.tv_nsec) / 1000000000.0;

  fps_data->prev_time = now;

  dt = clampd(dt, 1e-6, 0.25);

  fps_data->dt = dt;
  fps_data->fps_instant = 1.0 / dt;

  const double alpha = 0.20;
  if (fps_data->fps <= 0.0)
  {
    fps_data->fps = fps_data->fps_instant;
  }
  else
  {
    fps_data->fps = fps_data->fps + alpha * (fps_data->fps_instant - fps_data->fps);
  }

  snprintf(fps_data->fps_text, sizeof(fps_data->fps_text), "FPS: %.0f (%.0f)", fps_data->fps, fps_data->fps_instant);
}

int main(void)
{
  renderer_context_t *ctx = renderer_create(WIDTH, HEIGHT);
  float vertices[] = {MONKEY_VERTICES};
  uint32_t indices[] = {MONKEY_INDICES};

  renderer_text_style_t hud = {.size = 2, .fg = 0xFFFFFFFF, .bg = 0x80000000, .draw_bg = 1};

  fps_t fps_data = {0};
  app_state_t app = {.draw_help = 0, .draw_fps = 0, .wireframe_mode = 1, .paused = 0};

  ui_menu_bar_t menu;
  ui_menu_init(&menu, &hud);
  ui_menu_add(&menu, KEY_1, "Help", menu_toggle_help, &app);
  ui_menu_add(&menu, KEY_2, "FPS", menu_toggle_fps, &app);
  ui_menu_add(&menu, KEY_3, "Wireframe", menu_toggle_wireframe, &app);
  ui_menu_add(&menu, KEY_4, "Pause", menu_toggle_pause, &app);

  clock_gettime(CLOCK_MONOTONIC, &fps_data.prev_time);

  while (!platform_should_close())
  {
    update_fps(&fps_data);
    ui_menu_handle_input(&menu);

    const float move_speed = 0.005f;
    const float rot_speed = 0.003f;

    if (!app.paused)
    {
      float forward = 0.0f, right = 0.0f, up = 0.0f;
      if (platform_key_down(KEY_W))
      {
        forward -= move_speed;
      }
      if (platform_key_down(KEY_S))
      {
        forward += move_speed;
      }
      if (platform_key_down(KEY_D))
      {
        right += move_speed;
      }
      if (platform_key_down(KEY_A))
      {
        right -= move_speed;
      }
      if (platform_key_down(KEY_E))
      {
        up += move_speed;
      }
      if (platform_key_down(KEY_Q))
      {
        up -= move_speed;
      }

      if (forward != 0.0f || right != 0.0f || up != 0.0f)
      {
        renderer_camera_move_local(ctx, forward, right, up);
      }

      float yaw_delta = 0.0f, pitch_delta = 0.0f;
      if (platform_key_down(KEY_LEFT))
      {
        yaw_delta += rot_speed;
      }
      if (platform_key_down(KEY_RIGHT))
      {
        yaw_delta -= rot_speed;
      }
      if (platform_key_down(KEY_UP))
      {
        pitch_delta += rot_speed;
      }
      if (platform_key_down(KEY_DOWN))
      {
        pitch_delta -= rot_speed;
      }

      if (yaw_delta != 0.0f || pitch_delta != 0.0f)
      {
        renderer_camera_rotate(ctx, yaw_delta, pitch_delta);
      }
    }

    renderer_begin_frame(ctx);
    renderer_clean(ctx, 0xFF000000);

    ui_menu_draw(&menu, ctx);

    if (app.wireframe_mode)
    {
      renderer_begin(ctx, R_PRIMITIVE_LINE);
    }
    else
    {
      renderer_begin(ctx, R_PRIMITIVE_POINT);
    }
    for (size_t i = 0; i + 2 < MONKEY_INDEX_COUNT; i += 3)
    {
      float *a = &vertices[indices[i] * 3];
      float *b = &vertices[indices[i + 1] * 3];
      float *c = &vertices[indices[i + 2] * 3];

      renderer_vertex(ctx, a, 3);
      renderer_vertex(ctx, b, 3);
      renderer_vertex(ctx, b, 3);
      renderer_vertex(ctx, c, 3);
      renderer_vertex(ctx, c, 3);
      renderer_vertex(ctx, a, 3);
    }

    if (app.draw_help)
    {
      renderer_draw_text(ctx, 10, 40,
                         "W forward\nA left\nS backwards\nD right\nQ up\nE down\nArrows to rotate\n1 Help\n2 FPS\n3 "
                         "Wireframe\n4 Pause",
                         &hud);
    }

    if (app.draw_fps)
    {
      renderer_draw_text(ctx, 10, 250, fps_data.fps_text, &hud);
    }

    renderer_end_frame(ctx);
  }

  renderer_destroy(ctx);
  return 0;
}
