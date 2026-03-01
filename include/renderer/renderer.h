/**
 * @file renderer.h
 * @brief Software Renderer API. 
 * @copyright MIT License
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief framebuffer pixel type. Also used to pass into the platform abstraction layer.
 */
typedef uint32_t pixel_t;

/**
 * @brief opaque renderer_context structure, implemented in renderer.c.
 */
typedef struct renderer_context renderer_context_t;

/**
 * @brief Ways of displaying a group of vertices.
 */
typedef enum
{
    R_PRIMITIVE_POINT, /**< Just a single vertex in the screen. */
    R_PRIMITIVE_LINE /**< Line formed with the current and previous vertex passed with renderer_vertex function. */
} renderer_primitives_e;

/**
 * @brief Basic text styling.
 */
typedef struct renderer_text_style_t
{
    int size; /**< Each character pixel will be displayed with size * size dimention. */
    pixel_t fg; /**< Foreground color. */
    pixel_t bg; /**< Background color. */
    int draw_bg; /**< Tell if it is to draw the background color. */
} renderer_text_style_t;

/**
 * @brief Creates a renderer context and initializes the platform layer.
 *
 * Allocates the internal framebuffer and initializes all renderer state.
 * Internally calls platform_init().
 *
 * @param[in] width  Width of the framebuffer and display (in pixels).
 * @param[in] height Height of the framebuffer and display (in pixels).
 *
 * @return Pointer to a valid renderer_context_t on success,
 *         or NULL on failure.
 */
renderer_context_t *renderer_create(size_t width, size_t height);

/**
 * @brief Destroys a renderer context and shuts down the platform layer.
 *
 * Frees all allocated memory and internally calls platform_shutdown().
 *
 * @param[in] context Pointer to the renderer context.
 */
void renderer_destroy(renderer_context_t *context);

/**
 * @brief Begins a new frame.
 *
 * Processes platform events (input, window events, etc.)
 * by internally calling platform_process_events().
 *
 * Must be called once per frame before any drawing operations.
 *
 * @param[in] context Pointer to the renderer context.
 */
void renderer_begin_frame(renderer_context_t* context);

/**
 * @brief Ends the current frame and presents it to the display.
 *
 * Sends the framebuffer to the platform layer using platform_present().
 *
 * Must be called once per frame after drawing operations.
 *
 * @param[in] context Pointer to the renderer context.
 */
void renderer_end_frame(renderer_context_t* context);

/**
 * @brief Clears the framebuffer with a given color.
 *
 * Fills the entire framebuffer with the specified color.
 *
 * @param[in] context Pointer to the renderer context.
 * @param[in] color   Color used to fill all framebuffer pixels.
 */
void renderer_clean(renderer_context_t* context, pixel_t color);

/**
 * @brief Begins drawing primitives.
 *
 * Sets the current primitive type. All subsequent calls to
 * renderer_vertex() will follow this primitive mode until
 * renderer_begin() is called again.
 *
 * @param[in] context   Pointer to the renderer context.
 * @param[in] primitive Primitive type:
 *                      - R_PRIMITIVE_POINT
 *                      - R_PRIMITIVE_LINE
 */
void renderer_begin(renderer_context_t *context, renderer_primitives_e primitive);

/**
 * @brief Submits a vertex to the renderer.
 *
 * Interprets the provided vertex data according to the current primitive mode.
 * For lines, the previous vertex is stored internally to form segments.
 *
 * The expected layout of the vertex array depends on the renderer
 * implementation (commonly x, y, z or x, y).
 *
 * @param[in] context  Pointer to the renderer context.
 * @param[in] vertices Pointer to an array of floats representing the vertex.
 * @param[in] size     Number of floats in the vertices array.
 */
void renderer_vertex(renderer_context_t* context, float *vertices, size_t size);

/**
 * @brief Moves the camera in its local coordinate space.
 *
 * Movement is relative to the camera's orientation.
 *
 * @param[in] ctx      Pointer to the renderer context.
 * @param[in] forward  Movement along the forward axis.
 * @param[in] right    Movement along the right axis.
 * @param[in] up       Movement along the up axis.
 */
void renderer_camera_move_local(renderer_context_t* ctx, float forward, float right, float up);

/**
 * @brief Rotates the camera.
 *
 * @param[in] ctx         Pointer to the renderer context.
 * @param[in] yaw_delta   Change in yaw angle (horizontal rotation).
 * @param[in] pitch_delta Change in pitch angle (vertical rotation).
 */
void renderer_camera_rotate(renderer_context_t* ctx, float yaw_delta, float pitch_delta);

/**
 * @brief Draws text at a given screen position.
 *
 * Text is rendered in screen space using the provided style.
 * Coordinates are expressed in pixels relative to the top-left corner.
 *
 * @param[in] ctx   Pointer to the renderer context.
 * @param[in] x     Horizontal position in pixels.
 * @param[in] y     Vertical position in pixels.
 * @param[in] text  Null-terminated UTF-8 string to draw.
 * @param[in] style Pointer to text style configuration.
 */
void renderer_draw_text(renderer_context_t *ctx, int x, int y, const char *text, const renderer_text_style_t *style);

#endif // RENDERER_H

