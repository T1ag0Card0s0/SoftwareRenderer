#ifndef TEXT_H
#define TEXT_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t pixel_t;

typedef struct text_style_t {
    int scale;
    pixel_t fg;
    pixel_t bg;
    int draw_bg;
} text_style_t;

void text_draw(pixel_t *pixels, size_t width, size_t height,
               int x, int y, const char *text, const text_style_t *style);

#endif // TEXT_H
