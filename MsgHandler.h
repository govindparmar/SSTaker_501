#pragma once
#include <Windows.h>
#include "BMPTimer.h"
/**
 * CMsgHandler class - does the "dirty work" of the window procedure (handle messages etc).
 * Refactor #4 - existence of this class
 */
class CMsgHandler
{
private:
	// The *target window* of the bitmap captures. Must be static as this class is created/deconstructed for each message posted to the queue.
	static HWND hwTarget;
	// The child windows of the parent screenshot taking app window.
	HWND hWnd, hwCmdSource, hStartBtn, hStopBtn, hSelect, hStatic2;
	// The identifier of the message (WM_whatever)
	UINT Msg;
	// WPARAM and LPARAM passed to the message 
	WPARAM wParam;
	LPARAM lParam;
	// Bitmap timer class (necessary on start call)
	CBMPTimer *bmpT;
	// Return value
	LRESULT return_val;
public:
	// Refactor #5 - remove the various redundant calls to FindWindowEx with all but 1 identical parameters
	inline HWND FindWindowQ(LPCWSTR className, LPCWSTR winText);
	// The various message handlers.
	VOID Handle_Close();
	VOID Handle_Destroy();
	VOID Handle_Command();
	VOID Handle_Generic();

	// Sub-handlers for WM_COMMAND based on command source
	VOID Handle_Start();
	VOID Handle_Stop();
	VOID Handle_Select();

	// Return the return value (required for WindowProc stub)
	LRESULT Get_Return();

	// Constructor/deconstructor
	CMsgHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, HWND hStaticW);
	~CMsgHandler();
};

