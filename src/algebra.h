#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <stddef.h>

typedef struct
{
  float* values;
  size_t size;
} vec_t;

void algebra_rotate_x(vec_t vec, float angle);

void algebra_rotate_y(vec_t vec, float angle);

void algebra_rotate_z(vec_t vec, float angle);

void algebra_scale(vec_t vec, vec_t scale);

#endif // ALGEBRA_H
