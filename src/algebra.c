#include "algebra.h"

#include <math.h>
#include <string.h>

void mat4_identity(float matrix[16])
{
  memset(matrix, 0, sizeof(float) * 16);
  matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
}

void mat4_mul(float out[16], const float a[16], const float b[16])
{
  float r[16];
  for (int c = 0; c < 4; ++c)
  {
    for (int rrow = 0; rrow < 4; ++rrow)
    {
      r[c * 4 + rrow] = a[0 * 4 + rrow] * b[c * 4 + 0] + a[1 * 4 + rrow] * b[c * 4 + 1] +
                        a[2 * 4 + rrow] * b[c * 4 + 2] + a[3 * 4 + rrow] * b[c * 4 + 3];
    }
  }
  memcpy(out, r, sizeof(r));
}

void mat4_translate(float out[16], float tx, float ty, float tz)
{
  mat4_identity(out);
  out[12] = tx;
  out[13] = ty;
  out[14] = tz;
}

void mat4_perspective(float out[16], float fovy_rad, float aspect, float znear, float zfar)
{
  float f = 1.0f / tanf(fovy_rad * 0.5f);
  memset(out, 0, sizeof(float) * 16);
  out[0] = f / aspect;
  out[5] = f;
  out[10] = (zfar + znear) / (znear - zfar);
  out[11] = -1.0f;
  out[14] = (2.0f * zfar * znear) / (znear - zfar);
}

void mat4_mul_vec4(float out[4], const float m[16], const float v[4])
{
  out[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12] * v[3];
  out[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13] * v[3];
  out[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14] * v[3];
  out[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
}

void mat4_rot_x(float out[16], float a)
{
  mat4_identity(out);
  float c = cosf(a), s = sinf(a);
  out[5] = c;
  out[9] = -s;
  out[6] = s;
  out[10] = c;
}

void mat4_rot_y(float out[16], float a)
{
  mat4_identity(out);
  float c = cosf(a), s = sinf(a);
  out[0] = c;
  out[8] = s;
  out[2] = -s;
  out[10] = c;
}

void mat4_scale(float out[16], float sx, float sy, float sz)
{
  mat4_identity(out);
  out[0] = sx;
  out[5] = sy;
  out[10] = sz;
}
