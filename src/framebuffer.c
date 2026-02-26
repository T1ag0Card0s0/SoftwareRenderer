#include "framebuffer.h"
#include "renderer/renderer.h"

#include <stdlib.h>

#define DIRTY_REGIONS_MIX_WIDTH 16
#define DIRTY_REGIONS_MIN_HEIGHT 16

framebuffer_t framebuffer_create(int width, int height)
{
    framebuffer_t out = {0};
    
    out.height = height;
    out.width = width;
    out.pixels = (pixel_t*)calloc(width * height, sizeof(pixel_t));
    return out;
}

void framebuffer_destroy(framebuffer_t *framebuffer)
{
    free(framebuffer->pixels);
}

void framebuffer_clean(framebuffer_t* framebuffer, rect_t rect, pixel_t color)
{
    for(int i = rect.y; i < rect.y + rect.height; i++)
    {
        for(int j = rect.x; j < rect.x + rect.width; j++)
        {
            *(framebuffer->pixels + (i * framebuffer->width) + j) = color;
        }
    }
}

