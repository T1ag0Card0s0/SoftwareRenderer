#include "ui_menu.h"
#include "renderer/platform.h"
#include "renderer/renderer.h"

#include <stdio.h>
#include <string.h>

static int key_slot_from_key(int key)
{
  switch (key)
  {
  case KEY_1:
    return 0;
  case KEY_2:
    return 1;
  case KEY_3:
    return 2;
  case KEY_4:
    return 3;
  default:
    return -1;
  }
}

void ui_menu_init(ui_menu_bar_t *menu, const renderer_text_style_t *style)
{
  memset(menu, 0, sizeof(*menu));
  menu->bar_height = 24;
  menu->padding = 6;
  menu->spacing = 6;
  menu->bar_bg = 0xC0202020;
  menu->item_bg = 0x80404040;
  menu->item_active_bg = 0xC0606060;
  menu->active_index = -1;

  if (style)
  {
    menu->text_style = *style;
  }
}

int ui_menu_add(ui_menu_bar_t *menu, int key, const char *label, ui_menu_callback_t on_select, void *user_data)
{
  if (menu->count >= 4)
  {
    return 0;
  }

  int slot = key_slot_from_key(key);
  if (slot < 0)
  {
    return 0;
  }

  ui_menu_item_t *item = &menu->items[menu->count++];
  item->key = key;
  item->label = label;
  item->on_select = on_select;
  item->user_data = user_data;

  return 1;
}

void ui_menu_handle_input(ui_menu_bar_t *menu)
{
  for (size_t i = 0; i < menu->count; ++i)
  {
    int slot = key_slot_from_key(menu->items[i].key);
    if (slot < 0)
    {
      continue;
    }

    int down = platform_key_down(menu->items[i].key);
    int pressed = down && !menu->prev_keys[slot];

    if (pressed)
    {
      menu->active_index = (int)i;
      if (menu->items[i].on_select)
      {
        menu->items[i].on_select(menu->items[i].user_data);
      }
    }

    menu->prev_keys[slot] = down;
  }
}

void ui_menu_draw(ui_menu_bar_t *menu, renderer_context_t *ctx)
{
  int x = menu->padding;
  char text[128];

  for (size_t i = 0; i < menu->count; ++i)
  {
    snprintf(text, sizeof(text), "[%zu] %s", i + 1, menu->items[i].label);

    int item_w = (int)(strlen(text) * 8 * menu->text_style.size);

    renderer_draw_text(ctx, x + 6, 8, text, &menu->text_style);

    x += item_w + menu->spacing;
  }
}
