#include "camera.h"

#include "algebra.h"

#include <math.h>

void camera_build_view(float out_view[16], float px, float py, float pz, float yaw, float pitch)
{
  float Rx[16], Ry[16], T[16], tmp[16];

  mat4_rot_x(Rx, -pitch);
  mat4_rot_y(Ry, -yaw);
  mat4_translate(T, -px, -py, -pz);

  mat4_mul(tmp, Ry, T);
  mat4_mul(out_view, Rx, tmp);
}

void camera_forward(float out[3], float yaw, float pitch)
{
  out[0] = cosf(pitch) * sinf(yaw);
  out[1] = sinf(pitch);
  out[2] = cosf(pitch) * cosf(yaw);
}
