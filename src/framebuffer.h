#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "renderer/renderer.h"

#include <stddef.h>

typedef struct 
{
    int x;
    int y;
    int width;
    int height;
} rect_t;

typedef struct
{
    pixel_t *pixels;
    int width;
    int height;
} framebuffer_t;

framebuffer_t framebuffer_create(int width, int height);

void framebuffer_destroy(framebuffer_t *framebuffer);

void framebuffer_clean(framebuffer_t* framebuffer, rect_t rect, pixel_t color);

#endif // FRAMEBUFFER_H
