#ifndef PLATFORM_H
#define PLATFORM_H

#include "renderer.h"

#include <stdbool.h>

void platform_init(int width, int height);
int platform_process_events(void);
void platform_present(pixel_t *pixels, size_t width, size_t height);
void platform_shutdown(void);
bool platform_should_close(void);

#endif // !PLATFORM_H
