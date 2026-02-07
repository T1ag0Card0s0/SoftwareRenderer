#include "renderer/renderer.h"
#include "renderer/platform.h"

#include "algebra.h"
#include "camera.h"
#include "framebuffer.h"
#include "primitive.h"
#include "text.h"

#include <math.h>
#include <stdlib.h>

struct renderer_context
{
  framebuffer_t framebuffer;
  renderer_primitives_e primitive;

  float model[16];
  float proj[16];

  camera_t camera;
  float view[16];
};

renderer_context_t *renderer_create(size_t width, size_t height)
{
  renderer_context_t *out = (renderer_context_t *)calloc(1, sizeof(renderer_context_t));
  out->framebuffer = framebuffer_create(width, height);
  platform_init(width, height);

  mat4_identity(out->model);

  float aspect = (float)width / (float)height;
  mat4_perspective(out->proj, 60.0f * (float)M_PI / 180.0f, aspect, 0.1f, 100.0f);

  out->camera.pos[0] = 0.0f;
  out->camera.pos[1] = 0.0f;
  out->camera.pos[2] = 3.0f;
  out->camera.yaw = 0.0f;
  out->camera.pitch = 0.0f;

  camera_build_view(out->view, out->camera.pos[0], out->camera.pos[1], out->camera.pos[2], out->camera.yaw,
                    out->camera.pitch);

  return out;
}

void renderer_destroy(renderer_context_t *ctx)
{
  framebuffer_destroy(&ctx->framebuffer);
  free(ctx);
  platform_shutdown();
}

void renderer_begin_frame(renderer_context_t *context)
{
  (void)context;
}

void renderer_end_frame(renderer_context_t *context)
{
  platform_present(context->framebuffer.pixels, context->framebuffer.width, context->framebuffer.height);
}

void renderer_clean(renderer_context_t *context, pixel_t color)
{
  framebuffer_clean(
      &context->framebuffer,
      (rect_t){.width = context->framebuffer.width, .height = context->framebuffer.height, .x = 0, .y = 0}, color);
}

void renderer_begin(renderer_context_t *context, renderer_primitives_e primitive)
{
  context->primitive = primitive;
}

void renderer_vertex(renderer_context_t *context, float *vertex, size_t size)
{
  if (size % 3 != 0)
    return;

  void (*primitive_fn)(framebuffer_t *, float *, pixel_t) = 0;
  switch (context->primitive)
  {
  case R_PRIMITIVE_POINT:
    primitive_fn = &primitive_point;
    break;
  case R_PRIMITIVE_LINE:
    primitive_fn = &primitive_line;
    break;
  default:
    return;
  }

  float mv[16], mvp[16];
  mat4_mul(mv, context->view, context->model);
  mat4_mul(mvp, context->proj, mv);

  for (size_t i = 0; i + 2 < size; i += 3)
  {
    float v4[4] = {vertex[i + 0], vertex[i + 1], vertex[i + 2], 1.0f};
    float clip[4];
    mat4_mul_vec4(clip, mvp, v4);

    if (clip[3] == 0.0f)
      continue;

    float ndc[3] = {clip[0] / clip[3], clip[1] / clip[3], clip[2] / clip[3]};

    if (ndc[0] < -1.0f || ndc[0] > 1.0f || ndc[1] < -1.0f || ndc[1] > 1.0f || ndc[2] < -1.0f || ndc[2] > 1.0f)
      continue;

    primitive_fn(&context->framebuffer, ndc, 0xFF00FF00);
  }
}

void renderer_camera_move_local(renderer_context_t *ctx, float forward, float right, float up)
{
  float fx = sinf(ctx->camera.yaw);
  float fz = cosf(ctx->camera.yaw);

  float rx = cosf(ctx->camera.yaw);
  float rz = -sinf(ctx->camera.yaw);

  ctx->camera.pos[0] += fx * forward + rx * right;
  ctx->camera.pos[1] += up;
  ctx->camera.pos[2] += fz * forward + rz * right;

  camera_build_view(ctx->view, ctx->camera.pos[0], ctx->camera.pos[1], ctx->camera.pos[2], ctx->camera.yaw,
                    ctx->camera.pitch);
}

void renderer_camera_rotate(renderer_context_t *ctx, float yaw_delta, float pitch_delta)
{
  ctx->camera.yaw += yaw_delta;
  ctx->camera.pitch += pitch_delta;

  if (ctx->camera.pitch > 1.55f)
    ctx->camera.pitch = 1.55f;
  if (ctx->camera.pitch < -1.55f)
    ctx->camera.pitch = -1.55f;

  camera_build_view(ctx->view, ctx->camera.pos[0], ctx->camera.pos[1], ctx->camera.pos[2], ctx->camera.yaw,
                    ctx->camera.pitch);
}

void renderer_draw_text(renderer_context_t *ctx, int x, int y, const char *text, const renderer_text_style_t *style)
{
  if (!ctx || !text)
    return;

  text_style_t st;
  if (style)
  {
    st.scale = style->size > 0 ? style->size : 1;
    st.fg = style->fg;
    st.bg = style->bg;
    st.draw_bg = style->draw_bg;
  }
  else
  {
    st.scale = 1;
    st.fg = 0xFFFFFFFF;
    st.bg = 0x00000000;
    st.draw_bg = 0;
  }

  text_draw(ctx->framebuffer.pixels, ctx->framebuffer.width, ctx->framebuffer.height, x, y, text, &st);
}
