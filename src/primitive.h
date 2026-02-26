#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "framebuffer.h"
#include "renderer/renderer.h"

void primitive_point(framebuffer_t *framebuffer, float vertex[3], pixel_t color);
void primitive_line(framebuffer_t *framebuffer, float vertex[6], pixel_t color);

#endif //PRIMITIVE_H
