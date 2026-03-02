#include "renderer/platform.h"
#include <string.h>
#include <windows.h>

static HWND hwnd;
static BITMAPINFO bmi;
static bool running = true;
static bool keys[KEY_COUNT] = {0};

static keycode_t translate_key(WPARAM w)
{
  switch (w)
  {
  case 'W':
    return KEY_W;
  case 'A':
    return KEY_A;
  case 'S':
    return KEY_S;
  case 'D':
    return KEY_D;

  case 'Q':
    return KEY_Q;
  case 'E':
    return KEY_E;

  case VK_LEFT:
    return KEY_LEFT;
  case VK_RIGHT:
    return KEY_RIGHT;
  case VK_UP:
    return KEY_UP;
  case VK_DOWN:
    return KEY_DOWN;

  case VK_ESCAPE:
    return KEY_ESC;

  default:
    return KEY_COUNT;
  }
}

static LRESULT CALLBACK proc(HWND h, UINT m, WPARAM w, LPARAM l)
{
  switch (m)
  {
  case WM_CLOSE:
  case WM_DESTROY:
    running = false;
    PostQuitMessage(0);
    return 0;

  case WM_KEYDOWN: {
    keycode_t k = translate_key(w);
    if (k != KEY_COUNT)
    {
      keys[k] = true;
    }
    return 0;
  }

  case WM_KEYUP: {
    keycode_t k = translate_key(w);
    if (k != KEY_COUNT)
    {
      keys[k] = false;
    }
    return 0;
  }
  }

  return DefWindowProc(h, m, w, l);
}

void platform_init(int width, int height)
{
  WNDCLASS wc;
  memset(&wc, 0, sizeof(wc));

  wc.lpfnWndProc = proc;
  wc.hInstance = GetModuleHandle(NULL);
  wc.lpszClassName = "sw_renderer";

  RegisterClass(&wc);

  hwnd = CreateWindow(wc.lpszClassName, "Renderer", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
                      width, height, NULL, NULL, wc.hInstance, NULL);

  /* Setup framebuffer format */
  memset(&bmi, 0, sizeof(bmi));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height; // top-down DIB
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
}

void platform_process_events(void)
{
  MSG msg;

  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void platform_present(pixel_t *pixels, size_t width, size_t height)
{
  HDC dc = GetDC(hwnd);

  StretchDIBits(dc, 0, 0, (int)width, (int)height, 0, 0, (int)width, (int)height, pixels, &bmi, DIB_RGB_COLORS,
                SRCCOPY);

  ReleaseDC(hwnd, dc);
}

void platform_shutdown(void)
{
  if (hwnd)
  {
    DestroyWindow(hwnd);
    hwnd = NULL;
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