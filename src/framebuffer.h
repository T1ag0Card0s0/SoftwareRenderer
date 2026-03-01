/**
 * @file framebuffer.h
 * @brief Framebuffer abstraction.
 * @copyright MIT License
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "renderer/renderer.h"

#include <stddef.h>

/**
 * @brief Integer rectangle structure.
 *
 * Defines a rectangular region inside a framebuffer.
 * Coordinates are expressed in pixel units.
 */
typedef struct 
{
    int x; /**< Left coordinate (in pixels). */
    int y; /**< Top coordinate (in pixels). */
    int width; /**< Rectangle width (in pixels). */
    int height; /**< Rectangle height (in pixels). */
} rect_t;

/**
 * @brief Framebuffer structure.
 *
 * Stores a contiguous 2D pixel array. 
 * Pixels are laid out as:
 *
 *     pixels[y * width + x]
 */
typedef struct
{
    pixel_t *pixels; /**< Pointer to pixel data. The array contains width * height elements. */
    int width; /**< Framebuffer width in pixels. */
    int height; /**< Framebuffer height in pixels. */
} framebuffer_t;

/**
 * @brief Creates a framebuffer.
 *
 * Allocates memory for a pixel buffer of size width * height.
 * Pixel contents are undefined after creation.
 *
 * @param[in] width  Framebuffer width (must be > 0).
 * @param[in] height Framebuffer height (must be > 0).
 *
 * @return A framebuffer_t structure with allocated pixel memory.
 */
framebuffer_t framebuffer_create(int width, int height);

/**
 * @brief Destroys a framebuffer.
 *
 * Frees the internal pixel buffer and resets its fields.
 *
 * @param[in,out] framebuffer Pointer to the framebuffer.
 */
void framebuffer_destroy(framebuffer_t *framebuffer);

/**
 * @brief Clears a rectangular region of the framebuffer.
 *
 * Fills the specified rectangle with a given color.
 * The rectangle is expected to lie within framebuffer bounds.
 * Behavior is undefined if the rectangle exceeds the buffer.
 *
 * @param[in,out] framebuffer Pointer to the framebuffer.
 * @param[in]     rect        Rectangle region to clear.
 * @param[in]     color       Fill color.
 */
void framebuffer_clean(framebuffer_t* framebuffer, rect_t rect, pixel_t color);

#endif // FRAMEBUFFER_H
