#include "renderer/platform.h"
#include <windows.h>

static HWND hwnd;
static BITMAPINFO bmi;
static int running = 1;

LRESULT CALLBACK proc(HWND h, UINT m, WPARAM w, LPARAM l)
{
  if (m == WM_DESTROY)
    running = 0;
  return DefWindowProc(h, m, w, l);
}

void platform_init(int w, int h)
{
  WNDCLASS wc = {0};
  wc.lpfnWndProc = proc;
  wc.lpszClassName = "sw";
  RegisterClass(&wc);

  hwnd = CreateWindow("sw", "Renderer", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, w, h, 0, 0, GetModuleHandle(0), 0);

  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = w;
  bmi.bmiHeader.biHeight = -h;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
}

int platform_process_events()
{
  MSG msg;
  while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return running;
}

void platform_present(pixel_t *pixels, size_t width, size_t height)
{
  HDC dc = GetDC(hwnd);

  StretchDIBits(dc, 0, 0, width, height, 0, 0, width, height, pixels, &bmi, DIB_RGB_COLORS, SRCCOPY);

  ReleaseDC(hwnd, dc);
}

void platform_shutdown()
{
}
