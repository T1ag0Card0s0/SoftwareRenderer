#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <stddef.h>

void platform_init(int width, int height);

int platform_process_events(void);

void platform_present(uint32_t *pixels, size_t width, size_t height);

void platform_shutdown(void);

#endif // PLATFORM_H
