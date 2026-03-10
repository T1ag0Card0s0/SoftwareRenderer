/**
 * @file platform.h
 * @brief Platform abstraction layer.
 * @copyright MIT License
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#include "renderer.h"
#include "platform_types.h"
#include <stdbool.h>

typedef enum 
{
  KEY_W, KEY_A, KEY_S, KEY_D,
  KEY_Q, KEY_E,
  KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,
  KEY_ESC,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_COUNT
} keycode_t;

/**
 * @brief Specific initialization, mainly to start some window and enabling events. Called in renderer_create function.
 *
 * @param[in] width Width of the window, at least equal to the framebuffer width.
 * @param[in] height Height of the window, at least equal to the framebuffer height.
 */
void platform_init(int width, int height);

/**
 * @brief Processes all incomming events. Called in renderer_begin_frame function.
 */
void platform_process_events(void);

/**
 * @brief Shows the full frame in the display. Called at renderer_end_frame.
 *
 * @param[in] pixels pixel_t array, should contain width * height pixels.
 * @param[in] width Width of the framebuffer.
 * @param[in] height Height of the framebuffer.
 */
void platform_present(pixel_t *pixels, size_t width, size_t height);

/**
 * @brief Executes all the steps to safely stop the renderer. Called at renderer_destroy. 
 */
void platform_shutdown(void);

/**
 * @brief Tells if the platform should close. This function is not called inside the renderer.
 * @return true if should close, false otherwise
 */
bool platform_should_close(void);

/**
 * @brief Tells if a key was pressed.
 *
 * @param key keycode_t Key to check if was pressed.
 *
 * @return true if pressed, false otherwise.
 */
bool platform_key_down(keycode_t key);

int  platform_mutex_init   (platform_mutex_t *m);
void platform_mutex_destroy(platform_mutex_t *m);
void platform_mutex_lock   (platform_mutex_t *m);
void platform_mutex_unlock (platform_mutex_t *m);

int  platform_cond_init     (platform_cond_t *c);
void platform_cond_destroy  (platform_cond_t *c);
void platform_cond_wait     (platform_cond_t *c, platform_mutex_t *m);
void platform_cond_signal   (platform_cond_t *c);
void platform_cond_broadcast(platform_cond_t *c);

int  platform_thread_create(platform_thread_t *t, platform_thread_return_t (PLATFORM_THREAD_CALL *fn)(void *), void *arg);
void platform_thread_join(platform_thread_t t);
platform_time_t platform_time_now(void);
double platform_time_diff_ms(platform_time_t s, platform_time_t e);
void platform_sleep_ms(int ms);
unsigned long platform_thread_id(void);

#endif // PLATFORM_H
