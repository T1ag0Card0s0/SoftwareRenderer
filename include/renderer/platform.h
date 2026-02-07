#ifndef PLATFORM_H
#define PLATFORM_H

#include "renderer.h"

#include <stdbool.h>

typedef enum {
  KEY_W, KEY_A, KEY_S, KEY_D,
  KEY_Q, KEY_E,
  KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,
  KEY_ESC,
  KEY_COUNT
} keycode_t;

void platform_init(int width, int height);
int platform_process_events(void);
void platform_present(pixel_t *pixels, size_t width, size_t height);
void platform_shutdown(void);
bool platform_should_close(void);
bool platform_key_down(keycode_t key);

#endif // !PLATFORM_H
