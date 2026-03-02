#include "renderer/platform.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*PFNGLXSWAPINTERVALEXTPROC)(Display *, GLXDrawable, int);

static Display *display = NULL;
static Window window = 0;
static GLXContext context = NULL;
static Atom wm_delete;

static bool running = true;
static bool keys[KEY_COUNT] = {0};

static GLuint texture = 0;
static GLuint pbo[2] = {0, 0};
static int pbo_index = 0;

static int fb_width = 0;
static int fb_height = 0;

static keycode_t translate_key(KeySym sym)
{
  switch (sym)
  {
  case XK_w:
  case XK_W:
    return KEY_W;
  case XK_a:
  case XK_A:
    return KEY_A;
  case XK_s:
  case XK_S:
    return KEY_S;
  case XK_d:
  case XK_D:
    return KEY_D;

  case XK_q:
  case XK_Q:
    return KEY_Q;
  case XK_e:
  case XK_E:
    return KEY_E;

  case XK_Left:
    return KEY_LEFT;
  case XK_Right:
    return KEY_RIGHT;
  case XK_Up:
    return KEY_UP;
  case XK_Down:
    return KEY_DOWN;

  case XK_Escape:
    return KEY_ESC;
  default:
    return KEY_COUNT;
  }
}

void platform_init(int width, int height)
{
  fb_width = width;
  fb_height = height;

  display = XOpenDisplay(NULL);
  if (!display)
  {
    fprintf(stderr, "Failed to open X display\n");
    exit(1);
  }

  int screen = DefaultScreen(display);

  GLint attribs[] = {GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 24, None};

  XVisualInfo *vi = glXChooseVisual(display, screen, attribs);
  if (!vi)
  {
    fprintf(stderr, "No appropriate GLX visual found\n");
    exit(1);
  }

  Colormap cmap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);

  XSetWindowAttributes swa;
  swa.colormap = cmap;
  swa.event_mask = KeyPressMask | KeyReleaseMask | StructureNotifyMask;

  window = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, width, height, 0, vi->depth, InputOutput,
                         vi->visual, CWColormap | CWEventMask, &swa);

  XStoreName(display, window, "Fast Renderer");

  wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, window, &wm_delete, 1);

  XMapWindow(display, window);

  context = glXCreateContext(display, vi, NULL, GL_TRUE);
  if (!context)
  {
    fprintf(stderr, "Failed to create GLX context\n");
    exit(1);
  }

  glXMakeCurrent(display, window, context);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    fprintf(stderr, "Failed to initialize GLEW\n");
    exit(1);
  }
  PFNGLXSWAPINTERVALEXTPROC swapIntervalEXT =
      (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((const GLubyte *)"glXSwapIntervalEXT");

  if (swapIntervalEXT)
  {
    swapIntervalEXT(display, window, 0);
  }

  glDisable(GL_DEPTH_TEST);
  glViewport(0, 0, width, height);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glGenBuffers(2, pbo);

  for (int i = 0; i < 2; i++)
  {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo[i]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, NULL, GL_STREAM_DRAW);
  }

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void platform_process_events(void)
{
  while (XPending(display))
  {
    XEvent event;
    XNextEvent(display, &event);

    if (event.type == ClientMessage)
    {
      if ((Atom)event.xclient.data.l[0] == wm_delete)
      {
        running = false;
      }
    }
    else if (event.type == KeyPress)
    {
      keycode_t k = translate_key(XLookupKeysym(&event.xkey, 0));
      if (k != KEY_COUNT)
      {
        keys[k] = true;
      }
    }
    else if (event.type == KeyRelease)
    {
      keycode_t k = translate_key(XLookupKeysym(&event.xkey, 0));
      if (k != KEY_COUNT)
      {
        keys[k] = false;
      }
    }
  }
}

void platform_present(pixel_t *pixels, size_t width, size_t height)
{
  const size_t size = width * height * 4;
  int next = (pbo_index + 1) % 2;

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo[next]);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, size, NULL, GL_STREAM_DRAW);

  void *ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
  if (ptr)
  {
    memcpy(ptr, pixels, size);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
  }

  glBindTexture(GL_TEXTURE_2D, texture);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo[pbo_index]);

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)width, (GLsizei)height, GL_RGBA, GL_UNSIGNED_BYTE, 0);

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  pbo_index = next;

  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_TEXTURE_2D);

  glBegin(GL_QUADS);
  glTexCoord2f(0, 1);
  glVertex2f(-1, -1);
  glTexCoord2f(1, 1);
  glVertex2f(1, -1);
  glTexCoord2f(1, 0);
  glVertex2f(1, 1);
  glTexCoord2f(0, 0);
  glVertex2f(-1, 1);
  glEnd();

  glXSwapBuffers(display, window);
}

void platform_shutdown(void)
{
  if (pbo[0])
  {
    glDeleteBuffers(2, pbo);
  }
  if (texture)
  {
    glDeleteTextures(1, &texture);
  }

  if (context)
  {
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
  }

  if (window)
  {
    XDestroyWindow(display, window);
  }

  if (display)
  {
    XCloseDisplay(display);
  }
}

bool platform_should_close(void)
{
  return !running;
}

bool platform_key_down(keycode_t key)
{
  if (key >= KEY_COUNT)
  {
    return false;
  }

  return keys[key];
}
