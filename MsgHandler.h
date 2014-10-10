#pragma once
#include <Windows.h>
#include "BMPTimer.h"
class CMsgHandler
{
private:
	static HWND hwTarget;
	HWND hWnd, hwCmdSource, hStartBtn, hStopBtn, hSelect, hStatic2;
	UINT Msg;
	WPARAM wParam;
	LPARAM lParam;
	CBMPTimer *bmpT;
	LRESULT return_val;
public:
	inline HWND FindWindowQ(LPCWSTR className, LPCWSTR winText);
	VOID Handle_Close();
	VOID Handle_Destroy();
	VOID Handle_Command();
	VOID Handle_Start();
	VOID Handle_Stop();
	VOID Handle_Select();
	VOID Handle_Generic();
	LRESULT Get_Return();
	CMsgHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, HWND hStaticW);
	~CMsgHandler();
};

