#include <stdio.h>

#include "renderer/platform.h"
#include "renderer/renderer.h"

#define WIDTH 640
#define HEIGHT 480

int main(void) 
{
    renderer_context_t * ctx = renderer_create(WIDTH, HEIGHT);
    
    while(!platform_should_close())
    {
        renderer_begin(ctx);
        renderer_clean(ctx, 0xFF000000);
        renderer_end(ctx);
    }

    renderer_destroy(ctx);
    return 0;
}
