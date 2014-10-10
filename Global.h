#include "BMPTimer.h"
#include "BMPWriter.h"
#include "MsgHandler.h"
VOID CreateChildWindows();
ATOM RegisterWindowClass(HINSTANCE);
BOOL CALLBACK SysFontProc(HWND, LPARAM);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
const TCHAR szClassName[] = TEXT("ScreenSnapperWnd");
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SysFontProc(HWND, LPARAM);
