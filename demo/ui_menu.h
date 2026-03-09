#ifndef UI_MENU_H
#define UI_MENU_H

#include <stdint.h>
#include <stddef.h>

#include "renderer/renderer.h"

typedef void (*ui_menu_callback_t)(void *user_data);

typedef struct
{
  int key;
  const char *label;
  ui_menu_callback_t on_select;
  void *user_data;
} ui_menu_item_t;

typedef struct
{
  ui_menu_item_t items[4];
  size_t count;

  int bar_height;
  int padding;
  int spacing;

  renderer_text_style_t text_style;
  uint32_t bar_bg;
  uint32_t item_bg;
  uint32_t item_active_bg;

  int prev_keys[4];
  int active_index; 
} ui_menu_bar_t;

void ui_menu_init(ui_menu_bar_t *menu, const renderer_text_style_t *style);
int ui_menu_add(ui_menu_bar_t *menu, int key, const char *label, ui_menu_callback_t on_select, void *user_data);
void ui_menu_handle_input(ui_menu_bar_t *menu);
void ui_menu_draw(ui_menu_bar_t *menu, renderer_context_t *ctx);

#endif
