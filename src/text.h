/**
 * @file text.h
 * @brief Text rendering utilities for a software framebuffer.
 * @copyright MIT License
 */

#ifndef TEXT_H
#define TEXT_H

#include "framebuffer.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Text rendering style.
 *
 * Defines how characters are drawn onto a framebuffer.
 */
typedef struct text_style_t 
{
    int scale; /**< Character scale factor. Each character pixel is displayed with scale * scale pixels dimention. */
    pixel_t fg; /**< Foreground (text) color. */
    pixel_t bg; /**< Background color. */
    int draw_bg; /**< Whether to draw the background rectangle behind text. */
} text_style_t;

/**
 * @brief Draws text onto a framebuffer.
 *
 * Renders the provided string starting at the given screen coordinates.
 * Characters are scaled according to the provided style.
 * Background rectangles can be drawn depending on style->draw_bg.
 *
 * @param[in,out] framebuffer Target framebuffer.
 * @param[in]     x           Horizontal position in pixels (top-left corner).
 * @param[in]     y           Vertical position in pixels (top-left corner).
 * @param[in]     text        Null-terminated string to draw.
 * @param[in]     style       Pointer to text rendering style.
 */
void text_draw(framebuffer_t *framebuffer, int x, int y, const char *text, const text_style_t *style);

#endif // TEXT_H
