#include <stdint.h>
#include <stdlib.h>

#include "renderer/renderer.h"
#include "renderer/platform.h"

#define W 640
#define H 480

static uint32_t framebuffer[W*H];

int main()
{
    platform_init(W,H);

    int frame = 0;

    while(platform_process_events())
    {
        renderer_draw(framebuffer, W, H, frame++);
        platform_present(framebuffer);
    }

    platform_shutdown();
    return 0;
}

