#include "camera.h"
#include "algebra.h"
#include <math.h>

#define PITCH_LIMIT 1.55f

static void camera_rebuild_view(camera_t *cam)
{
  float Rx[16], Ry[16], T[16], tmp[16];
  mat4_rot_x(Rx, -cam->pitch);
  mat4_rot_y(Ry, -cam->yaw);
  mat4_translate(T, -cam->pos[0], -cam->pos[1], -cam->pos[2]);
  mat4_mul(tmp, Ry, T);
  mat4_mul(cam->view, Rx, tmp);
}

void camera_init(camera_t *cam, float x, float y, float z)
{
  cam->pos[0] = x;
  cam->pos[1] = y;
  cam->pos[2] = z;
  cam->yaw = 0.0f;
  cam->pitch = 0.0f;
  camera_rebuild_view(cam);
}

void camera_move_local(camera_t *cam, float forward, float right, float up)
{
  float fx = sinf(cam->yaw);
  float fz = cosf(cam->yaw);
  float rx = cosf(cam->yaw);
  float rz = -sinf(cam->yaw);

  cam->pos[0] += fx * forward + rx * right;
  cam->pos[1] += up;
  cam->pos[2] += fz * forward + rz * right;

  camera_rebuild_view(cam);
}

void camera_rotate(camera_t *cam, float dyaw, float dpitch)
{
  cam->yaw += dyaw;
  cam->pitch += dpitch;

  if (cam->pitch > PITCH_LIMIT)
  {
    cam->pitch = PITCH_LIMIT;
  }
  if (cam->pitch < -PITCH_LIMIT)
  {
    cam->pitch = -PITCH_LIMIT;
  }

  camera_rebuild_view(cam);
}

const float *camera_get_view(const camera_t *cam)
{
  return cam->view;
}
