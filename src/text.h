#ifndef TEXT_H
#define TEXT_H

#include "framebuffer.h"

#include <stddef.h>
#include <stdint.h>

typedef struct text_style_t 
{
    int scale;
    pixel_t fg;
    pixel_t bg;
    int draw_bg;
} text_style_t;

void text_draw(framebuffer_t *framebuffer, int x, int y, const char *text, const text_style_t *style);

#endif // TEXT_H
