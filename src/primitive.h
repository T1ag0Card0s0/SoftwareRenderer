/**
 * @file primitive.h
 * @brief Low-level drawing primitives.
 * @copyright MIT License
 */

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "framebuffer.h"
#include "renderer/renderer.h"

/**
 * @brief Draws a single point.
 *
 * @param[in,out] framebuffer Target framebuffer.
 * @param[in]     vertex      3-component vertex (x, y, z).
 * @param[in]     color       Pixel color.
 */
void primitive_point(framebuffer_t *framebuffer, float vertex[3], pixel_t color);

/**
 * @brief Draws a line between two vertices.
 *
 * Rasterizes a line segment between two 3D vertices projected
 * into screen space.
 *
 * The vertex format is:
 *
 *     vertex[0] = x0
 *     vertex[1] = y0
 *     vertex[2] = z0
 *     vertex[3] = x1
 *     vertex[4] = y1
 *     vertex[5] = z1
 *
 * Bresenham line rasterization algorithm.
 *
 * @param[in,out] framebuffer Target framebuffer.
 * @param[in]     vertex      6-component array defining two endpoints.
 * @param[in]     color       Pixel color.
 */
void primitive_line(framebuffer_t *framebuffer, float vertex[6], pixel_t color);

#endif //PRIMITIVE_H
