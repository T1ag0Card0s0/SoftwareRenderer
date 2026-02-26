#include "renderer/platform.h"
#include "renderer/renderer.h"
#include "resources/monkey.h"
#define WIDTH 640
#define HEIGHT 480

int main(void) 
{
    renderer_context_t * ctx = renderer_create(WIDTH, HEIGHT);
    float array[] = {MONKEY}; 
    while(!platform_should_close())
    {
        renderer_begin_frame(ctx);
        renderer_clean(ctx, 0xFF000000);
        renderer_begin(ctx, R_PRIMITIVE_POINT);
        renderer_vertex(ctx, array, MONKEY_FLOATS);
        renderer_end_frame(ctx);
    }

    renderer_destroy(ctx);
    return 0;
}
