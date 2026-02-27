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
  mat4_perspective(out->proj, 60.0f * (float)M_PI / 180.0f, aspect, 0.1f, 1000.0f);

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
  case R_PRIMITIVE_WIRE_FRAME:
    primitive_fn = &primitive_wireframe;
    break;
  default:
    return;
  }

  float mv[16], mvp[16];
  mat4_mul(mv, context->view, context->model);
  mat4_mul(mvp, context->proj, mv);

    if(context->primitive == R_PRIMITIVE_POINT)
    {

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
    else if(context->primitive == R_PRIMITIVE_WIRE_FRAME)
    {

  for (size_t i = 0; i + 8 < size; i += 9)
  {
    float triangle[9];

    for (int v = 0; v < 3; ++v)
    {
      float v4[4] = {
        vertex[i + v*3 + 0],
        vertex[i + v*3 + 1],
        vertex[i + v*3 + 2],
        1.0f
      };

      float clip[4];
      mat4_mul_vec4(clip, mvp, v4);

      if (clip[3] == 0.0f)
        continue;

      float inv_w = 1.0f / clip[3];

      triangle[v*3 + 0] = clip[0] * inv_w;
      triangle[v*3 + 1] = clip[1] * inv_w;
      triangle[v*3 + 2] = clip[2] * inv_w;
    }

    primitive_wireframe(&context->framebuffer, triangle, 0xFF00FF00);
  }
    }
}

// helper: transform object-space position -> NDC (x,y,z)
static int transform_to_ndc(float out_ndc[3], const float mvp[16], float x, float y, float z)
{
  float v4[4] = {x, y, z, 1.0f};
  float clip[4];
  mat4_mul_vec4(clip, mvp, v4);

  if (clip[3] == 0.0f)
    return 0;

  float inv_w = 1.0f / clip[3];
  out_ndc[0] = clip[0] * inv_w;
  out_ndc[1] = clip[1] * inv_w;
  out_ndc[2] = clip[2] * inv_w;

  // simple clip (same as your point path)
  if (out_ndc[0] < -1.0f || out_ndc[0] > 1.0f ||
      out_ndc[1] < -1.0f || out_ndc[1] > 1.0f ||
      out_ndc[2] < -1.0f || out_ndc[2] > 1.0f)
    return 0;

  return 1;
}

void renderer_draw_indexed(renderer_context_t *ctx,
                           const float *vertices, size_t vertex_floats,
                           const uint32_t *indices, size_t index_count)
{
  if (!ctx || !vertices || !indices)
    return;
  if ((vertex_floats % 3) != 0)
    return;
  if ((index_count % 3) != 0)
    return;

  // only meaningful for triangles right now
  if (ctx->primitive != R_PRIMITIVE_WIRE_FRAME)
    return;

  float mv[16], mvp[16];
  mat4_mul(mv, ctx->view, ctx->model);
  mat4_mul(mvp, ctx->proj, mv);

  const size_t vertex_count = vertex_floats / 3;

  for (size_t i = 0; i + 2 < index_count; i += 3)
  {
    uint32_t ia = indices[i + 0];
    uint32_t ib = indices[i + 1];
    uint32_t ic = indices[i + 2];

    if (ia >= vertex_count || ib >= vertex_count || ic >= vertex_count)
      continue;

    float tri_ndc[9];
    int ok0 = transform_to_ndc(&tri_ndc[0], mvp,
                              vertices[ia * 3 + 0], vertices[ia * 3 + 1], vertices[ia * 3 + 2]);
    int ok1 = transform_to_ndc(&tri_ndc[3], mvp,
                              vertices[ib * 3 + 0], vertices[ib * 3 + 1], vertices[ib * 3 + 2]);
    int ok2 = transform_to_ndc(&tri_ndc[6], mvp,
                              vertices[ic * 3 + 0], vertices[ic * 3 + 1], vertices[ic * 3 + 2]);

    // super simple: if any vertex is clipped, skip the triangle
    // (later you can do proper line clipping)
    if (!(ok0 && ok1 && ok2))
      continue;

    primitive_wireframe(&ctx->framebuffer, tri_ndc, 0xFF00FF00);
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
