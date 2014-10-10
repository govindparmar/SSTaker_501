// The below files include my self-written classes for this project.
#include "BMPWriter.h"
#include "BMPTimer.h"
#include "MsgHandler.h"
// The below two files allow for necessary Windows API and Process API functions in this project
#include <Windows.h>
#include <Psapi.h>

// Class name - required for all Windows APIs
const TCHAR szClassName[] = TEXT("ScreenSnapperWnd");

// Function prototypes
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SysFontProc(HWND, LPARAM);
HWND hwTarget = (HWND)0x0000000, hStaticWI = (HWND)0x00000000, hWnd, hStart, hStop, hFindTarget, hTimerMS, hStaticE;

/** Refactor #1:
 * Given the length of WinMain, extract some methods from it, namely, the initialization of the WNDCLASSEX structure 
 */
ATOM RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInstance;
	wcex.style = CS_BYTEALIGNWINDOW | CS_BYTEALIGNCLIENT | CS_GLOBALCLASS;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szClassName;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	return RegisterClassEx(&wcex);
}

/**
 * Still Refactor #1 - extract child Window initialization from WinMain into a new method.
 */
#define WS_RCHILD (WS_VISIBLE | WS_CHILD)
VOID CreateChildWindows()
{
	hStaticE = CreateWindow(TEXT("STATIC"), TEXT("Enter the interval between screenshots (milliseconds):"), WS_RCHILD, 0, 0, 270, 20, hWnd, NULL, GetModuleHandle(NULL), NULL);
	hTimerMS = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("5000"), WS_RCHILD | ES_NUMBER, 0, 22, 270, 20, hWnd, NULL, GetModuleHandle(NULL), NULL);
	hFindTarget = CreateWindow(TEXT("BUTTON"), TEXT("Select Window"), WS_RCHILD | BS_TEXT, 10, 44, 250, 30, hWnd, NULL, GetModuleHandle(NULL), NULL);
	hStaticWI = CreateWindow(TEXT("STATIC"), TEXT(""), WS_RCHILD | SS_LEFT, 0, 88, 270, 40, hWnd, NULL, GetModuleHandle(NULL), NULL);
	hStart = CreateWindow(TEXT("BUTTON"), TEXT("Start"), WS_RCHILD | BS_TEXT, 10, 130, 125, 30, hWnd, NULL, GetModuleHandle(NULL), NULL);
	hStop = CreateWindow(TEXT("BUTTON"), TEXT("Stop"), WS_RCHILD | BS_TEXT | WS_DISABLED, 135, 130, 125, 30, hWnd, NULL, GetModuleHandle(NULL), NULL);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	
	MSG Msg;

	if(!RegisterWindowClass(hInstance))
	{
		MessageBox(0, L"Window Registration Failed!", L"Error", MB_OK|MB_ICONSTOP);
		return -1;
	}
	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, szClassName, TEXT("Screenshot Taker"), WS_VISIBLE | WS_SYSMENU, 100, 100, 290, 210, NULL, NULL, GetModuleHandle(NULL), NULL);
	CreateChildWindows();
	
	ShowWindow(hWnd, SW_SHOW);
	EnumChildWindows(hWnd, SysFontProc, 0);
	UpdateWindow(hWnd);
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
BOOL CALLBACK SysFontProc(HWND hWnd, LPARAM lParam)
{
	HFONT hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hfDefault, 0);
	return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	CMsgHandler msghandler(hWnd, Msg, wParam, lParam, hStaticWI);
	return msghandler.Get_Return();
}