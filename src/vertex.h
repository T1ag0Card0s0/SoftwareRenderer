#ifndef VERTEX_H
#define VERTEX_H

#include "renderer/renderer.h"
#include "framebuffer.h"

void vec3f_put_pixel(framebuffer_t *fb, vec3f_t v, color_t color);

#endif // VERTEX_H
