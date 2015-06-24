#include "MsgHandler.h"
// Macros for eliminating duplicated code - simplifies method calls to FindWindowEx
#define FINDBUTTON(wtxt) (FindWindowQ((TEXT("BUTTON")), (L##wtxt)))
#define FINDEDIT (FindWindowQ((TEXT("EDIT")), (NULL)))
HWND CMsgHandler::hwTarget;
// Inline method for handling calls to FindWIndowEx - removes necessity for several duplicated calls etc
inline HWND CMsgHandler::FindWindowQ(LPCWSTR className, LPCWSTR winText)
{
	return FindWindowEx(hWnd, NULL, className, winText);
}

// Handles the WM_CLOSE message (destroys the window, return 0)
VOID CMsgHandler::Handle_Close()
{
	DestroyWindow(hWnd);
	return_val = (LRESULT)0;
}

// Handles the WM_DESTROY message (posts the exit code for the window; return 0)
VOID CMsgHandler::Handle_Destroy()
{
	PostQuitMessage(0);
	return_val = (LRESULT)0;
}

// Stub method to handle all calls not specifically supported by our class (call DefWindowProc)
VOID CMsgHandler::Handle_Generic()
{
	return_val = DefWindowProc(hWnd, Msg, wParam, lParam);
}

// Handles the WM_COMMAND message
VOID CMsgHandler::Handle_Command()
{
	// Step 1: identify the source of the command (which button)
	 hwCmdSource = (HWND)lParam;
	// Step 2: pass to the appropriate sub-handle for WM_COMMAND based on the button
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

// Class constructor for CMsgHandler 
//  sets up member variables and determines which message handler to use
CMsgHandler::CMsgHandler(HWND ahWnd, UINT aMsg, WPARAM awParam, LPARAM alParam, HWND hStaticW)
{
	hWnd = ahWnd;
	Msg = aMsg;
	wParam = awParam;
	lParam = alParam;
	
	hStartBtn = FINDBUTTON("Start");
	hStopBtn = FINDBUTTON("Stop");
	hSelect = FINDBUTTON("Select Window");

	hStatic2 = hStaticW;
	if (Msg == WM_CLOSE) Handle_Close();
	else if (Msg == WM_DESTROY) Handle_Destroy();
	else if (Msg == WM_COMMAND) Handle_Command();
	else Handle_Generic();
}

// Sub handler for "select window" WM_COMMAND message source
// Asks the user for the window they want to target,
//  prints out info about target window on app window
VOID CMsgHandler::Handle_Select()
{
	MessageBox(0, TEXT("If you are targetting a usual Windows application, try to target the *title bar* of the application.  If you are targetting a Java or Flash applet target anywhere within the applet.\n\nPress OK and then hover your mouse over the target window within 2 seconds... (before pressing OK, you can move this message box closer to your target if need be)"), TEXT("Info"), MB_OK | MB_ICONINFORMATION);
	ShowWindow(hWnd, SW_MINIMIZE);
	Sleep(2000);
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

// Handles the "Start" WM_COMMAND message
// 1. Toggles the enabled state of start and stop button
// 2. intializes a CBMPTimer with the specified target and specified interval.
VOID CMsgHandler::Handle_Start()
{
	EnableWindow((HWND)lParam, FALSE);
	EnableWindow(FINDBUTTON("Stop"), TRUE);
	int len = GetWindowTextLength((FINDEDIT)) + 1;
	TCHAR *lenBuf = new TCHAR[len];
	GetWindowText(FINDEDIT, lenBuf, len);
	int ms = _wtoi(lenBuf);
	if (ms == 0)ms++;
	bmpT = new CBMPTimer(hwTarget, ms);
}

// Handles a "Stop" WM_COMMAND message
// 1. Stops the running CBMPTimer 
// 2. Toggles the enabled state of start and stop button
 
VOID CMsgHandler::Handle_Stop()
{
	bmpT->StopTimer();
	EnableWindow((HWND)lParam, FALSE);
	EnableWindow(FINDBUTTON("Start"), TRUE);
}

CMsgHandler::~CMsgHandler()
{

}
// Returns the stored LRESULT to WindowProc stub.
LRESULT CMsgHandler::Get_Return()
{
	return return_val;
}