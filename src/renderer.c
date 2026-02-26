#include "renderer/renderer.h"

#include "framebuffer.h"
#include "renderer/platform.h"

#include <stdlib.h>

struct renderer_context 
{
    framebuffer_t framebuffer;
};

renderer_context_t *renderer_create(size_t width, size_t height)
{
    renderer_context_t *out = (renderer_context_t*)calloc(1, sizeof(renderer_context_t));
    out->framebuffer = framebuffer_create(width, height);
    platform_init(width, height);
    return out;
}

void renderer_destroy(renderer_context_t *ctx)
{
    framebuffer_destroy(&ctx->framebuffer);
    free(ctx);
    platform_shutdown();
}

void renderer_begin(renderer_context_t* context)
{
    (void) context;
    platform_process_events();
}

void renderer_end(renderer_context_t* context)
{
    platform_present(context->framebuffer.pixels, context->framebuffer.width, context->framebuffer.width);
}

void renderer_clean(renderer_context_t* context, pixel_t color)
{
    framebuffer_clean(&context->framebuffer, (rect_t){.width = context->framebuffer.width, .height = context->framebuffer.height, .x = 0, .y = 0}, color);
}

