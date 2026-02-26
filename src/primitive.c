#include "primitive.h"

#define X_INDEX 0
#define Y_INDEX 1

void primitive_point(framebuffer_t *framebuffer, float vertex[3], pixel_t color)
{
    int x = (int)(((vertex[X_INDEX] + 1.0f) * 0.5f) * (framebuffer->width - 1));
    int y = (int)((1.0f - ((vertex[Y_INDEX] + 1.0f) * 0.5f)) * (framebuffer->height - 1));

    if (x < 0 || x >= framebuffer->width ||
        y < 0 || y >= framebuffer->height)
        return;

    framebuffer->pixels[y * framebuffer->width + x] = color;
}
