#include "renderer/platform.h"
#include <X11/Xlib.h>
#include <stdlib.h>
#include <string.h>

static Display *d;
static Window w;
static GC gc;
static XImage *img;
static int width, height;

void platform_init(int W, int H)
{
  width = W;
  height = H;

  d = XOpenDisplay(NULL);
  int s = DefaultScreen(d);

  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, W, H, 1, BlackPixel(d, s), WhitePixel(d, s));

  XSelectInput(d, w, KeyPressMask);
  XMapWindow(d, w);

  gc = DefaultGC(d, s);

  img = XCreateImage(d, DefaultVisual(d, s), 24, ZPixmap, 0, malloc(W * H * 4), W, H, 32, 0);
}

int platform_process_events()
{
  while (XPending(d))
  {
    XEvent e;
    XNextEvent(d, &e);
    if (e.type == KeyPress)
      return 0;
  }
  return 1;
}

void platform_present(uint32_t *pixels)
{
  memcpy(img->data, pixels, width * height * 4);
  XPutImage(d, w, gc, img, 0, 0, 0, 0, width, height);
}

void platform_shutdown()
{
}
