#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>

void platform_init(int w, int h);
int platform_process_events();
void platform_present(uint32_t *pixels);
void platform_shutdown();

#endif // PLATFORM_H
