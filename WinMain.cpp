// The below files include my self-written classes for this project.
#include "BMPWriter.h"
#include "BMPTimer.h"
#include "FileNameGenerator.h"

// The below two files allow for necessary Windows API and Process API functions in this project
#include <Windows.h>
#include <Psapi.h>

// Class name - required for all Windows APIs
const TCHAR szClassName[] = TEXT("ScreenSnapperWnd");

// Function prototypes
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SysFontProc(HWND, LPARAM);
HWND hwTarget = (HWND)0x0000000, hStaticWI = (HWND)0x00000000; 

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wcex;
	HWND hWnd, hStart, hStop, hFindTarget, hTimerMS, hStaticE;
	MSG Msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.style = CS_BYTEALIGNWINDOW | CS_BYTEALIGNCLIENT | CS_GLOBALCLASS;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szClassName;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	RegisterClassEx(&wcex);

	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, szClassName, TEXT("Screenshot Taker"), WS_VISIBLE | WS_SYSMENU, 100, 100, 290, 210, NULL, NULL, GetModuleHandle(NULL), NULL);
	hStaticE = CreateWindow(TEXT("STATIC"), TEXT("Enter the interval between screenshots (milliseconds):"), WS_VISIBLE | WS_CHILD, 0, 0, 270, 20, hWnd, NULL, GetModuleHandle(NULL), NULL);
	hTimerMS = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("5000"), WS_VISIBLE | WS_CHILD | ES_NUMBER, 0, 22, 270, 20, hWnd, NULL, GetModuleHandle(NULL), NULL);
	hFindTarget = CreateWindow(TEXT("BUTTON"), TEXT("Select Window"), WS_VISIBLE | WS_CHILD | BS_TEXT, 10, 44, 250, 30, hWnd, NULL, GetModuleHandle(NULL), NULL);
	hStaticWI = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | SS_LEFT, 0, 88, 270, 40, hWnd, NULL, GetModuleHandle(NULL), NULL);
	hStart = CreateWindow(TEXT("BUTTON"), TEXT("Start"), WS_VISIBLE | WS_CHILD | BS_TEXT, 10, 130, 125, 30, hWnd, NULL, GetModuleHandle(NULL), NULL);
	hStop = CreateWindow(TEXT("BUTTON"), TEXT("Stop"), WS_VISIBLE | WS_CHILD | BS_TEXT | WS_DISABLED, 135, 130, 125, 30, hWnd, NULL, GetModuleHandle(NULL), NULL);

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
	CBMPTimer *bmpT = NULL;
	switch (Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		if ((HWND)lParam == FindWindowEx(hWnd, NULL, TEXT("BUTTON"), TEXT("Select Window")))
		{
			MessageBox(0, TEXT("If you are targetting a usual Windows application, try to target the *title bar* of the application.  If you are targetting a Java or Flash applet target anywhere within the applet.\n\nPress OK and then hover your mouse over the target window within 5 seconds... (you can move this message box closer to your target if need be)"), TEXT("Info"), MB_OK | MB_ICONINFORMATION);
			ShowWindow(hWnd, SW_MINIMIZE);
			Sleep(5000);
			POINT p;
			GetCursorPos(&p);
			hwTarget = WindowFromPoint(p); 
			TCHAR wndTxt[255];
			SendMessage(hwTarget, WM_GETTEXT, 255, (LPARAM)wndTxt);
			TCHAR sttcbuf[1024];
			RECT wRect;
			GetWindowRect(hwTarget, &wRect);
			wsprintf(sttcbuf, TEXT("Target Window: %s\n(handle = 0x%.8X; rect=(%d,%d)-(%d,%d))"), wndTxt, hwTarget, wRect.left,wRect.top, wRect.right,wRect.bottom);
			SetWindowText(hStaticWI, sttcbuf);
			ShowWindow(hWnd, SW_RESTORE);
		}
		else if ((HWND)lParam == FindWindowEx(hWnd, NULL, TEXT("BUTTON"), TEXT("Start")))
		{
			EnableWindow((HWND)lParam, FALSE);
			EnableWindow(FindWindowEx(hWnd, NULL, TEXT("BUTTON"), TEXT("Stop")), TRUE);
			int len = GetWindowTextLength(FindWindowEx(hWnd, NULL, TEXT("EDIT"), NULL))+1;
			TCHAR *lenBuf = new TCHAR[len];
			GetWindowText(FindWindowEx(hWnd, NULL, TEXT("EDIT"), NULL), lenBuf, len);
			int ms = _wtoi(lenBuf);
			MessageBox(0, lenBuf, L"", MB_OK);
			if (ms == 0)ms++;
			bmpT = new CBMPTimer(hwTarget, ms);
		}
		else if ((HWND)lParam == FindWindowEx(hWnd, NULL, TEXT("BUTTON"), TEXT("Stop")))
		{
			bmpT->StopTimer();
			EnableWindow((HWND)lParam, FALSE);
			EnableWindow(FindWindowEx(hWnd, NULL, TEXT("BUTTON"), TEXT("Start")), TRUE);
		}
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}