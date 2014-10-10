#pragma once
#include <Windows.h>
class CMsgHandler
{
private:
	HWND hWnd, hwCmdSource, hStartBtn, hStopBtn, hSelect, hwTarget, hStatic2;
	UINT Msg;
	WPARAM wParam;
	LPARAM lParam;
	CBMPTimer *bmpT;
public:
	LRESULT Handle_Close();
	LRESULT Handle_Destroy();
	LRESULT Handle_Command();
	VOID Handle_Start();
	VOID Handle_Stop();
	VOID Handle_Select();
	LRESULT Handle_Generic();
	CMsgHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, HWND hStaticW);
	~CMsgHandler();
};

