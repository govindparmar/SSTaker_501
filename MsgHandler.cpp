#include "MsgHandler.h"
HWND CMsgHandler::hwTarget;
VOID CMsgHandler::Handle_Close()
{
	DestroyWindow(hWnd);
	return_val = (LRESULT)0;
}
VOID CMsgHandler::Handle_Destroy()
{
	PostQuitMessage(0);
	return_val = (LRESULT)0;
}

VOID CMsgHandler::Handle_Generic()
{
	return_val = DefWindowProc(hWnd, Msg, wParam, lParam);
}

VOID CMsgHandler::Handle_Command()
{
	 hwCmdSource = (HWND)lParam;
	
	if (hwCmdSource == hSelect)
	{
		Handle_Select();
	}
	if (hwCmdSource == hStartBtn)
	{
		if (hwTarget == NULL)
		{
			MessageBox(hWnd, TEXT("Please first select a target window!"), TEXT("Problem"), MB_OK | MB_ICONWARNING);
			return_val = (LRESULT)0;
			return;
		}
		Handle_Start();
	}
	if (hwCmdSource == hStopBtn)
	{
		Handle_Stop();
	}
	return_val = (LRESULT)0;
}

CMsgHandler::CMsgHandler(HWND ahWnd, UINT aMsg, WPARAM awParam, LPARAM alParam, HWND hStaticW)
{
	hWnd = ahWnd;
	Msg = aMsg;
	wParam = awParam;
	lParam = alParam;
	
	hStartBtn = FindWindowEx(hWnd, NULL, TEXT("BUTTON"), TEXT("Start"));
	hStopBtn = FindWindowEx(hWnd, NULL, TEXT("BUTTON"), TEXT("Stop"));
	hSelect = FindWindowEx(hWnd, NULL, TEXT("BUTTON"), TEXT("Select Window"));
	hStatic2 = hStaticW;
	if (Msg == WM_CLOSE) Handle_Close();
	else if (Msg == WM_DESTROY) Handle_Destroy();
	else if (Msg == WM_COMMAND) Handle_Command();
	else Handle_Generic();
}

VOID CMsgHandler::Handle_Select()
{
	MessageBox(0, TEXT("If you are targetting a usual Windows application, try to target the *title bar* of the application.  If you are targetting a Java or Flash applet target anywhere within the applet.\n\nPress OK and then hover your mouse over the target window within 2 seconds... (before pressing OK, you can move this message box closer to your target if need be)"), TEXT("Info"), MB_OK | MB_ICONINFORMATION);
	ShowWindow(hWnd, SW_MINIMIZE);
	Sleep(3000);
	POINT p;
	GetCursorPos(&p);
	hwTarget = WindowFromPoint(p);
	TCHAR wndTxt[255];
	SendMessage(hwTarget, WM_GETTEXT, 255, (LPARAM)wndTxt);
	TCHAR sttcbuf[1024];
	RECT wRect;
	GetWindowRect(hwTarget, &wRect);
	wsprintf(sttcbuf, TEXT("Target Window: %s\n(handle = 0x%.8X; rect=(%d,%d)-(%d,%d))"), wndTxt, hwTarget, wRect.left, wRect.top, wRect.right, wRect.bottom);
	SetWindowText(hStatic2, sttcbuf);
	ShowWindow(hWnd, SW_RESTORE);

}

VOID CMsgHandler::Handle_Start()
{
	EnableWindow((HWND)lParam, FALSE);
	EnableWindow(FindWindowEx(hWnd, NULL, TEXT("BUTTON"), TEXT("Stop")), TRUE);
	int len = GetWindowTextLength(FindWindowEx(hWnd, NULL, TEXT("EDIT"), NULL)) + 1;
	TCHAR *lenBuf = new TCHAR[len];
	GetWindowText(FindWindowEx(hWnd, NULL, TEXT("EDIT"), NULL), lenBuf, len);
	int ms = _wtoi(lenBuf);
	if (ms == 0)ms++;
	bmpT = new CBMPTimer(hwTarget, ms);
}

VOID CMsgHandler::Handle_Stop()
{
	bmpT->StopTimer();
	EnableWindow((HWND)lParam, FALSE);
	EnableWindow(FindWindowEx(hWnd, NULL, TEXT("BUTTON"), TEXT("Start")), TRUE);
}

CMsgHandler::~CMsgHandler()
{
	WM_CREATE;
}

LRESULT CMsgHandler::Get_Return()
{
	return return_val;
}