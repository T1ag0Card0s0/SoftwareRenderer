#include "renderer/renderer.h"
#include "renderer/platform.h"

#include "framebuffer.h"
#include "primitive.h"

#include <stdlib.h>

struct renderer_context 
{
    framebuffer_t framebuffer;
    renderer_primitives_e primitive;
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

void renderer_begin_frame(renderer_context_t* context)
{
    (void) context;
    platform_process_events();
}

void renderer_end_frame(renderer_context_t* context)
{
    platform_present(context->framebuffer.pixels, context->framebuffer.width, context->framebuffer.width);
}

void renderer_clean(renderer_context_t* context, pixel_t color)
{
    framebuffer_clean(&context->framebuffer, (rect_t){.width = context->framebuffer.width, .height = context->framebuffer.height, .x = 0, .y = 0}, color);
}

void renderer_begin(renderer_context_t* context, renderer_primitives_e primitive)
{
    context->primitive = primitive;
}

void renderer_vertex(renderer_context_t* context, float *vertex, size_t size)
{
   if(size % 3 != 0) return;
    void (*primitive_fn)(framebuffer_t *, float *, pixel_t );
    switch (context->primitive) 
    {
        case R_PRIMITIVE_POINT:
            primitive_fn = &primitive_point;
            break;
        case R_PRIMITIVE_LINE:
        break;
        default:
        break;
    }
    
    for(size_t i = 0; i <= size - 3; i = i + 3)
    {
        primitive_fn(&context->framebuffer, &vertex[i], 0xFF00FF00);
    }
}
