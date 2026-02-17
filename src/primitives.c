#include "primitives.h"

#include "renderer/config.h"

typedef struct
{
  size_t x;
  size_t y;
} coord_t;

static inline coord_t primitive_vec3f_to_coord(vec3f_t vec, framebuffer_t *fb)
{
  float z = vec.z + 3.0f;

  float px = vec.x / z;
  float py = vec.y / z;

  coord_t c;
  c.x = (size_t)((px + 1.0f) * 0.5f * (fb->width - 1));
  c.y = (size_t)((1.0f - (py + 1.0f) * 0.5f) * (fb->height - 1));
  return c;
}

void primitive_point(framebuffer_t *fb, vertex_t vertex)
{
  coord_t c = primitive_vec3f_to_coord(vertex.vec, fb);

  size_t r = POINT_RADIUS;
  size_t r2 = r * r;

  size_t x_start = (c.x > r) ? c.x - r : 0;
  size_t y_start = (c.y > r) ? c.y - r : 0;
  size_t x_end = ((c.x + r) < fb->width) ? c.x + r : fb->width - 1;
  size_t y_end = ((c.y + r) < fb->height) ? c.y + r : fb->height - 1;

  for (size_t py = y_start; py <= y_end; py++)
  {
    for (size_t px = x_start; px <= x_end; px++)
    {
      size_t dx = (px > c.x) ? px - c.x : c.x - px;
      size_t dy = (py > c.y) ? py - c.y : c.y - py;
      if (dx * dx + dy * dy <= r2)
      {
        fb->pixels[py * fb->width + px] = vertex.color;
      }
    }
  }
}

void primitive_line(framebuffer_t *fb, vertex_t vertices[2])
{
  coord_t c0 = primitive_vec3f_to_coord(vertices[0].vec, fb);
  coord_t c1 = primitive_vec3f_to_coord(vertices[1].vec, fb);

  int x0 = (int)c0.x;
  int y0 = (int)c0.y;
  int x1 = (int)c1.x;
  int y1 = (int)c1.y;

  int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
  int sx = (x0 < x1) ? 1 : -1;
  int dy = -((y1 > y0) ? (y1 - y0) : (y0 - y1));
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx + dy;

  while (1)
  {
    if (x0 >= 0 && x0 < (int)fb->width && y0 >= 0 && y0 < (int)fb->height)
    {
      fb->pixels[(size_t)y0 * fb->width + (size_t)x0] = vertices[0].color;
    }

    if (x0 == x1 && y0 == y1)
      break;

    int e2 = 2 * err;

    if (e2 >= dy)
    {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx)
    {
      err += dx;
      y0 += sy;
    }
  }
}

static int edge_function(int x0, int y0, int x1, int y1, int x, int y)
{
  return (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
}

void primitive_triangle(framebuffer_t *fb, vertex_t vertices[3])
{
  coord_t c0 = primitive_vec3f_to_coord(vertices[0].vec, fb);
  coord_t c1 = primitive_vec3f_to_coord(vertices[1].vec, fb);
  coord_t c2 = primitive_vec3f_to_coord(vertices[2].vec, fb);
  int x0 = (int)c0.x;
  int y0 = (int)c0.y;
  int x1 = (int)c1.x;
  int y1 = (int)c1.y;
  int x2 = (int)c2.x;
  int y2 = (int)c2.y;

  int min_x = x0 < x1 ? (x0 < x2 ? x0 : x2) : (x1 < x2 ? x1 : x2);
  int max_x = x0 > x1 ? (x0 > x2 ? x0 : x2) : (x1 > x2 ? x1 : x2);
  int min_y = y0 < y1 ? (y0 < y2 ? y0 : y2) : (y1 < y2 ? y1 : y2);
  int max_y = y0 > y1 ? (y0 > y2 ? y0 : y2) : (y1 > y2 ? y1 : y2);
  if (min_x < 0)
    min_x = 0;
  if (min_y < 0)
    min_y = 0;
  if (max_x >= (int)fb->width)
    max_x = fb->width - 1;
  if (max_y >= (int)fb->height)
    max_y = fb->height - 1;

  int area = edge_function(x0, y0, x1, y1, x2, y2);
  if (area <= 0)
    return;

  color_t col0 = vertices[0].color;
  color_t col1 = vertices[1].color;
  color_t col2 = vertices[2].color;

  for (int y = min_y; y <= max_y; y++)
  {
    for (int x = min_x; x <= max_x; x++)
    {
      int w0 = edge_function(x1, y1, x2, y2, x, y);
      int w1 = edge_function(x2, y2, x0, y0, x, y);
      int w2 = edge_function(x0, y0, x1, y1, x, y);
      if (w0 >= 0 && w1 >= 0 && w2 >= 0)
      {
        float a0 = (float)w0 / area;
        float a1 = (float)w1 / area;
        float a2 = (float)w2 / area;

        uint8_t r = (uint8_t)(a0 * COLOR_R(col0) + a1 * COLOR_R(col1) + a2 * COLOR_R(col2));
        uint8_t g = (uint8_t)(a0 * COLOR_G(col0) + a1 * COLOR_G(col1) + a2 * COLOR_G(col2));
        uint8_t b = (uint8_t)(a0 * COLOR_B(col0) + a1 * COLOR_B(col1) + a2 * COLOR_B(col2));

        fb->pixels[(size_t)y * fb->width + (size_t)x] = COLOR_RGB(r, g, b);
      }
    }
  }
}
