#include "renderer/renderer.h"

#include <math.h>

void renderer_rotateX(vec3f_t *v, size_t n, float angle)
{
  float c = cos(angle);
  float s = sin(angle);

  for (size_t i = 0; i < n; i++)
  {
    float y = v[i].y;
    float z = v[i].z;

    v[i].y = y * c - z * s;
    v[i].z = y * s + z * c;
  }
}

void renderer_rotateY(vec3f_t *v, size_t n, float angle)
{
  float c = cos(angle);
  float s = sin(angle);

  for (size_t i = 0; i < n; i++)
  {
    float x = v[i].x;
    float z = v[i].z;

    v[i].x = x * c + z * s;
    v[i].z = -x * s + z * c;
  }
}

void renderer_rotateZ(vec3f_t *v, size_t n, float angle)
{
  float c = cos(angle);
  float s = sin(angle);

  for (size_t i = 0; i < n; i++)
  {
    float x = v[i].x;
    float y = v[i].y;

    v[i].x = x * c - y * s;
    v[i].y = x * s + y * c;
  }
}

void renderer_scale(vec3f_t *v, size_t n, float scale, vec3f_t center)
{
  for (size_t i = 0; i < n; i++)
  {
    float x = v[i].x - center.x;
    float y = v[i].y - center.y;
    float z = v[i].z - center.z;

    x *= scale;
    y *= scale;
    z *= scale;

    v[i].x = x + center.x;
    v[i].y = y + center.y;
    v[i].z = z + center.z;
  }
}
