#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <stddef.h>

void mat4_identity(float matrix[16]);
void mat4_mul(float out[16], const float a[16], const float b[16]);
void mat4_translate(float out[16], float tx, float ty, float tz);
void mat4_perspective(float out[16], float fovy_rad, float aspect, float znear, float zfar);
void mat4_mul_vec4(float out[4], const float m[16], const float v[4]);
void mat4_rot_y(float out[16], float a);
void mat4_rot_x(float out[16], float a);
void mat4_scale(float out[16], float sx, float sy, float sz);

#endif // ALGEBRA_H
