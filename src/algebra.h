/**
 * @file algebra.h
 * @brief Minimal 4x4 matrix algebra utilities.
 * @copyright MIT License
 */

#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <stddef.h>

/**
 * @brief Sets a 4x4 matrix to the identity matrix.
 *
 * The resulting matrix has 1s on the diagonal and 0s elsewhere.
 *
 * @param[out] matrix Output 4x4 matrix (16 floats).
 */
void mat4_identity(float matrix[16]);

/**
 * @brief Multiplies two 4x4 matrices.
 *
 * Performs standard matrix multiplication:
 *
 *     out = a * b
 *
 * The result may safely alias a or b.
 *
 * @param[out] out Resulting 4x4 matrix.
 * @param[in]  a   Left-hand matrix.
 * @param[in]  b   Right-hand matrix.
 */
void mat4_mul(float out[16], const float a[16], const float b[16]);

/**
 * @brief Creates a translation matrix.
 *
 * The resulting matrix represents a translation by (tx, ty, tz).
 *
 * @param[out] out Output 4x4 translation matrix.
 * @param[in]  tx  Translation along X axis.
 * @param[in]  ty  Translation along Y axis.
 * @param[in]  tz  Translation along Z axis.
 */
void mat4_translate(float out[16], float tx, float ty, float tz);

/**
 * @brief Creates a perspective projection matrix.
 *
 * Constructs a right-handed perspective projection matrix.
 *
 * @param[out] out      Output 4x4 projection matrix.
 * @param[in]  fovy_rad Vertical field of view in radians.
 * @param[in]  aspect   Aspect ratio (width / height).
 * @param[in]  znear    Near clipping plane distance (must be > 0).
 * @param[in]  zfar     Far clipping plane distance (must be > znear).
 */
void mat4_perspective(float out[16], float fovy_rad, float aspect, float znear, float zfar);

/**
 * @brief Multiplies a 4x4 matrix by a 4D vector.
 *
 * Performs:
 *
 *     out = m * v
 *
 * @param[out] out Resulting 4D vector.
 * @param[in]  m   4x4 matrix.
 * @param[in]  v   4D input vector.
 */
void mat4_mul_vec4(float out[4], const float m[16], const float v[4]);

/**
 * @brief Creates a rotation matrix around the Y axis.
 *
 * @param[out] out Output 4x4 rotation matrix.
 * @param[in]  a   Rotation angle in radians.
 */
void mat4_rot_y(float out[16], float a);

/**
 * @brief Creates a rotation matrix around the X axis.
 *
 * @param[out] out Output 4x4 rotation matrix.
 * @param[in]  a   Rotation angle in radians.
 */
void mat4_rot_x(float out[16], float a);

/**
 * @brief Creates a scaling matrix.
 *
 * The resulting matrix scales along each axis independently.
 *
 * @param[out] out Output 4x4 scaling matrix.
 * @param[in]  sx  Scale factor along X axis.
 * @param[in]  sy  Scale factor along Y axis.
 * @param[in]  sz  Scale factor along Z axis.
 */
void mat4_scale(float out[16], float sx, float sy, float sz);


#endif // ALGEBRA_H
