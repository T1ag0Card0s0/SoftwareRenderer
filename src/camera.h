#ifndef CAMERA_H
#define CAMERA_H

typedef struct camera_t 
{
    float pos[3];
    float yaw;
    float pitch;
} camera_t;

void camera_build_view(float out_view[16], float px, float py, float pz, float yaw, float pitch);
void camera_forward(float out[3], float yaw, float pitch);

#endif // CAMERA_H
