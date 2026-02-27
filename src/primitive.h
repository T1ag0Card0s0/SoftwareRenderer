#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "framebuffer.h"
#include "renderer/renderer.h"

void primitive_point(framebuffer_t *framebuffer, float vertex[3], pixel_t color);
void primitive_wireframe(framebuffer_t *framebuffer, float vertex[9], pixel_t color);

#endif //PRIMITIVE_H
