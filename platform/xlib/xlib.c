#include "renderer/platform.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Display *d;
static Window w;
static GC gc;
static XImage *img;

static int win_w, win_h;
static bool should_close = false;
static Atom wm_delete_window;

static bool g_keys[KEY_COUNT];

static void set_key_state(KeySym ks, bool down)
{
  switch (ks)
  {
  case XK_w:
  case XK_W:
    g_keys[KEY_W] = down;
    break;
  case XK_a:
  case XK_A:
    g_keys[KEY_A] = down;
    break;
  case XK_s:
  case XK_S:
    g_keys[KEY_S] = down;
    break;
  case XK_d:
  case XK_D:
    g_keys[KEY_D] = down;
    break;

  case XK_q:
  case XK_Q:
    g_keys[KEY_Q] = down;
    break;
  case XK_e:
  case XK_E:
    g_keys[KEY_E] = down;
    break;

  case XK_Escape:
    g_keys[KEY_ESC] = down;
    break;
  case XK_Left:
    g_keys[KEY_LEFT] = down;
    break;
  case XK_Right:
    g_keys[KEY_RIGHT] = down;
    break;
  case XK_Up:
    g_keys[KEY_UP] = down;
    break;
  case XK_Down:
    g_keys[KEY_DOWN] = down;
    break;
  default:
    break;
  }
}

void platform_init(int W, int H)
{
  win_w = W;
  win_h = H;

  d = XOpenDisplay(NULL);
  if (!d)
  {
    fprintf(stderr, "XOpenDisplay failed\n");
    exit(1);
  }

  int s = DefaultScreen(d);

  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, (unsigned)W, (unsigned)H, 1, BlackPixel(d, s), WhitePixel(d, s));

  XSelectInput(d, w, KeyPressMask | KeyReleaseMask | StructureNotifyMask);
  XMapWindow(d, w);

  wm_delete_window = XInternAtom(d, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(d, w, &wm_delete_window, 1);

  gc = DefaultGC(d, s);

  img = XCreateImage(d, DefaultVisual(d, s), 24, ZPixmap, 0, (char *)malloc((size_t)W * (size_t)H * 4), (unsigned)W,
                     (unsigned)H, 32, 0);

  if (!img || !img->data)
  {
    fprintf(stderr, "XCreateImage/malloc failed\n");
    exit(1);
  }

  should_close = false;
  memset(g_keys, 0, sizeof(g_keys));
}

bool platform_should_close(void)
{
  return should_close;
}

bool platform_key_down(keycode_t key)
{
  if ((int)key < 0 || key >= KEY_COUNT)
    return false;
  return g_keys[key];
}

int platform_process_events()
{
  while (XPending(d))
  {
    XEvent e;
    XNextEvent(d, &e);

    if (e.type == KeyPress || e.type == KeyRelease)
    {
      bool down = (e.type == KeyPress);

      KeySym ks = XLookupKeysym(&e.xkey, 0);
      set_key_state(ks, down);

      if (down && ks == XK_Escape)
        should_close = true;
    }
    else if (e.type == ClientMessage)
    {
      if ((Atom)e.xclient.data.l[0] == wm_delete_window)
        should_close = true;
    }
    else if (e.type == DestroyNotify)
    {
      should_close = true;
    }
  }

  return should_close ? 0 : 1;
}

void platform_present(uint32_t *pixels, size_t width, size_t height)
{
  if (should_close)
    return;

  size_t wcopy = width;
  size_t hcopy = height;
  if ((int)wcopy > win_w)
    wcopy = (size_t)win_w;
  if ((int)hcopy > win_h)
    hcopy = (size_t)win_h;

  memcpy(img->data, pixels, wcopy * hcopy * 4);
  XPutImage(d, w, gc, img, 0, 0, 0, 0, (unsigned)wcopy, (unsigned)hcopy);
  XFlush(d);
}

void platform_shutdown()
{
  if (!d)
    return;

  if (img)
  {
    XDestroyImage(img);
    img = NULL;
  }

  if (w)
  {
    XDestroyWindow(d, w);
    w = 0;
  }

  XCloseDisplay(d);
  d = NULL;
}
