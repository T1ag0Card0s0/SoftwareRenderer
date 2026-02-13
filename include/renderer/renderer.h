#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>

#include "framebuffer.h"
#include "platform.h"

void sr_renderer_init(uint32_t *pixels, size_t width, size_t height);

void sr_renderer_finish(void);

void sr_renderer_end_frame(uint32_t fps);

void sr_renderer_start_face();

void sr_renderer_end_face();

void sr_rendere_vertex(vertex_t v);

#endif // RENDERER_H
