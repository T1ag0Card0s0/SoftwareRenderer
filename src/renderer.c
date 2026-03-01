#include "renderer/renderer.h"
#include "renderer/platform.h"

#include "algebra.h"
#include "camera.h"
#include "framebuffer.h"
#include "primitive.h"
#include "text.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

struct renderer_context
{
  framebuffer_t framebuffer;
  renderer_primitives_e primitive;

  float model[16];
  float proj[16];
  float view[16];

  camera_t camera;

  float cached_vertices[9];
  uint8_t cached_vertex_count;
};

renderer_context_t *renderer_create(size_t width, size_t height)
{
  renderer_context_t *ctx = (renderer_context_t *)calloc(1, sizeof(renderer_context_t));
  ctx->framebuffer = framebuffer_create(width, height);
  platform_init(width, height);

  mat4_identity(ctx->model);

  float aspect = (float)width / (float)height;
  mat4_perspective(ctx->proj, 60.0f * (float)M_PI / 180.0f, aspect, 0.1f, 100.0f);

  camera_init(&ctx->camera, 0.0f, 0.0f, 3.0f);
  memcpy(ctx->view, camera_get_view(&ctx->camera), 16 * sizeof(float));

  return ctx;
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
  platform_process_events();
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
  context->cached_vertex_count = 0;
}

void renderer_vertex(renderer_context_t *context, float *vertex, size_t size)
{
  if (size != 3)
  {
    return;
  }

  float mv[16], mvp[16];
  mat4_mul(mv, context->view, context->model);
  mat4_mul(mvp, context->proj, mv);

  float v4[4] = {vertex[0], vertex[1], vertex[2], 1.0f};
  float clip[4];
  mat4_mul_vec4(clip, mvp, v4);

  if (clip[3] == 0.0f)
  {
    return;
  }

  float ndc[3] = {clip[0] / clip[3], clip[1] / clip[3], clip[2] / clip[3]};

  if (ndc[0] < -1.0f || ndc[0] > 1.0f || ndc[1] < -1.0f || ndc[1] > 1.0f || ndc[2] < -1.0f || ndc[2] > 1.0f)
  {
    return;
  }

  if (context->primitive == R_PRIMITIVE_POINT)
  {
    primitive_point(&context->framebuffer, ndc, 0xFF00FF00);
  }
  else if (context->primitive == R_PRIMITIVE_LINE)
  {
    size_t base = context->cached_vertex_count * 3;
    context->cached_vertices[base + 0] = ndc[0];
    context->cached_vertices[base + 1] = ndc[1];
    context->cached_vertices[base + 2] = ndc[2];
    context->cached_vertex_count++;

    if (context->cached_vertex_count == 2)
    {
      primitive_line(&context->framebuffer, context->cached_vertices, 0xFF00FF00);
      context->cached_vertex_count = 0;
    }
  }
}

void renderer_camera_move_local(renderer_context_t *ctx, float forward, float right, float up)
{
  camera_move_local(&ctx->camera, forward, right, up);
  memcpy(ctx->view, camera_get_view(&ctx->camera), 16 * sizeof(float));
}

void renderer_camera_rotate(renderer_context_t *ctx, float dyaw, float dpitch)
{
  camera_rotate(&ctx->camera, dyaw, dpitch);
  memcpy(ctx->view, camera_get_view(&ctx->camera), 16 * sizeof(float));
}

void renderer_draw_text(renderer_context_t *ctx, int x, int y, const char *text, const renderer_text_style_t *style)
{
  if (!ctx || !text)
  {
    return;
  }

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

  text_draw(&ctx->framebuffer, x, y, text, &st);
}
