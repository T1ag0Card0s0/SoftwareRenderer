#ifndef CAMERA_H
#define CAMERA_H

typedef struct
{
    float pos[3];
    float yaw;
    float pitch;
    float view[16];
} camera_t;

void camera_init(camera_t *cam, float x, float y, float z);
void camera_move_local(camera_t *cam, float forward, float right, float up);
void camera_rotate(camera_t *cam, float dyaw, float dpitch);
const float *camera_get_view(const camera_t *cam);

#endif // CAMERA_H
