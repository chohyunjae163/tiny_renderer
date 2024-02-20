#pragma warning(disable: 4668)
#ifndef UNICODE
#define UNICODE
#endif


#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#pragma warning(push)
#pragma warning(disable: 4100)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance , PWSTR pCmdLine, int nCmdShow) {

  const wchar_t CLASS_NAME[] = L"Sameple Window Class";

  WNDCLASS wc = { 0 };

  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;

  RegisterClass(&wc);

  HWND hwnd = CreateWindowEx(
    0,
    CLASS_NAME,
    L"Learn to Program Windows",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
    NULL,
    NULL,
    hInstance,
    NULL
  );

  if ( hwnd == NULL){
    return 0;
  }

  ShowWindow(hwnd,nCmdShow);

  MSG msg = { NULL };

  while ( GetMessage(&msg,NULL,0,0) > 0) { 
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
    
  return 0;
}
#pragma warning(pop)

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch ( uMsg ) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);

      FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));

      EndPaint(hwnd,&ps);
    }
    return 0;
  }
  return DefWindowProc(hwnd,uMsg,wParam,lParam);
}
