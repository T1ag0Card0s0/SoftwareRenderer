#ifndef CAMERA_H
#define CAMERA_H

/**
 * @brief Camera structure.
 *
 * Stores position, orientation (yaw/pitch),
 * and the computed view matrix.
 *
 * Yaw   = rotation around the Y axis (left/right look).
 * Pitch = rotation around the X axis (up/down look).
 */
typedef struct
{
    float pos[3]; /**< Camera position in world space (x, y, z). */
    float yaw; /**< Yaw angle in radians. */
    float pitch; /**< Pitch angle in radians. */
    float view[16]; /**< Cached 4x4 view matrix. */
} camera_t;

/**
 * @brief Initializes the camera.
 *
 * Sets the initial position and resets orientation
 * to a default forward-facing direction.
 * The view matrix is computed automatically.
 *
 * @param[out] cam Pointer to camera structure.
 * @param[in]  x   Initial X position.
 * @param[in]  y   Initial Y position.
 * @param[in]  z   Initial Z position.
 */
void camera_init(camera_t *cam, float x, float y, float z);

/**
 * @brief Moves the camera in local space.
 *
 * Movement is relative to the current orientation:
 *
 *  - forward : along the forward vector
 *  - right   : along the right vector
 *  - up      : along the world up or local up direction
 *
 * Automatically updates the view matrix.
 *
 * @param[in,out] cam     Pointer to camera structure.
 * @param[in]     forward Forward displacement.
 * @param[in]     right   Right displacement.
 * @param[in]     up      Up displacement.
 */
void camera_move_local(camera_t *cam, float forward, float right, float up);

/**
 * @brief Rotates the camera.
 *
 * Updates yaw and pitch angles.
 * Pitch is typically clamped internally to avoid flipping.
 * Automatically updates the view matrix.
 *
 * @param[in,out] cam   Pointer to camera structure.
 * @param[in]     dyaw  Change in yaw angle (radians).
 * @param[in]     dpitch Change in pitch angle (radians).
 */
void camera_rotate(camera_t *cam, float dyaw, float dpitch);

/**
 * @brief Returns the current view matrix.
 *
 * The returned pointer remains valid as long as
 * the camera structure exists.
 *
 * @param[in] cam Pointer to camera structure.
 *
 * @return Pointer to 4x4 view matrix.
 */
const float *camera_get_view(const camera_t *cam);

#endif // CAMERA_H
