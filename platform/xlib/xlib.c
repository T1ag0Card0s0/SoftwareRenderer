#include "renderer/platform.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
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

  w = XCreateSimpleWindow(
      d, RootWindow(d, s),
      10, 10, (unsigned)W, (unsigned)H,
      1, BlackPixel(d, s), WhitePixel(d, s));

  XSelectInput(d, w, KeyPressMask | StructureNotifyMask);
  XMapWindow(d, w);

  wm_delete_window = XInternAtom(d, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(d, w, &wm_delete_window, 1);

  gc = DefaultGC(d, s);

  img = XCreateImage(
      d, DefaultVisual(d, s),
      24, ZPixmap,
      0, (char *)malloc((size_t)W * (size_t)H * 4),
      (unsigned)W, (unsigned)H,
      32, 0);

  if (!img || !img->data)
  {
    fprintf(stderr, "XCreateImage/malloc failed\n");
    exit(1);
  }

  should_close = false;
}

bool platform_should_close(void)
{
  return should_close;
}

int platform_process_events()
{
  while (XPending(d))
  {
    XEvent e;
    XNextEvent(d, &e);

    if (e.type == KeyPress)
    {
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
  if (should_close) return;

  size_t wcopy = width;
  size_t hcopy = height;
  if ((int)wcopy > win_w) wcopy = (size_t)win_w;
  if ((int)hcopy > win_h) hcopy = (size_t)win_h;

  memcpy(img->data, pixels, wcopy * hcopy * 4);
  XPutImage(d, w, gc, img, 0, 0, 0, 0, (unsigned)wcopy, (unsigned)hcopy);
  XFlush(d);
}

void platform_shutdown()
{
  if (!d) return;

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
