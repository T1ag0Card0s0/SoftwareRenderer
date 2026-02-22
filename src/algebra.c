#include "algebra.h"
#include <math.h>

static void algebra_for_each_XYZ(vec_t vec, void (*callback)(vec_t triple, void *user_data), void *user_data)
{
  for (size_t i = 0; i < vec.size; i += 3)
  {
    callback((vec_t){.values = vec.values + i, .size = 3}, user_data);
  }
}

static void algebra_multiply(vec_t triple, void *user_data)
{
  vec_t *matrix = (vec_t *)user_data;

  float x = triple.values[0];
  float y = triple.values[1];
  float z = triple.values[2];

  triple.values[0] = matrix->values[0] * x + matrix->values[1] * y + matrix->values[2] * z;
  triple.values[1] = matrix->values[3] * x + matrix->values[4] * y + matrix->values[5] * z;
  triple.values[2] = matrix->values[6] * x + matrix->values[7] * y + matrix->values[8] * z;
}
void algebra_rotate_x(vec_t vec, float angle)
{
  float c = cosf(angle);
  float s = sinf(angle);

  float matrix[9] = {1, 0, 0, 0, c, -s, 0, s, c};

  vec_t mul_vec = {.values = matrix, .size = 9};
  algebra_for_each_XYZ(vec, algebra_multiply, &mul_vec);
}

void algebra_rotate_y(vec_t vec, float angle)
{
  float c = cosf(angle);
  float s = sinf(angle);

  float matrix[9] = {c, 0, s, 0, 1, 0, -s, 0, c};

  vec_t mul_vec = (vec_t){.values = matrix, .size = 9};

  algebra_for_each_XYZ(vec, algebra_multiply, &mul_vec);
}

void algebra_rotate_z(vec_t vec, float angle)
{
  float c = cosf(angle);
  float s = sinf(angle);

  float matrix[9] = {c, -s, 0, s, c, 0, 0, 0, 1};

  vec_t mul_vec = (vec_t){.values = matrix, .size = 9};

  algebra_for_each_XYZ(vec, algebra_multiply, &mul_vec);
}
