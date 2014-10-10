#include "BMPTimer.h"
bool CBMPTimer::timeractive;

CBMPTimer::CBMPTimer(HWND hWnd, int ms)
{
	hwTarget = hWnd;
	ms_interval = ms;
	thistimer = reinterpret_cast<LPVOID>(this);
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StaticThreadProc, thistimer, 0, NULL);
	timeractive = true;
	ticks_so_far=0;
}

CBMPTimer::~CBMPTimer()
{
	delete thistimer;
}
DWORD CBMPTimer::StaticThreadProc(LPVOID lpParam)
{
	CBMPTimer *pThis = reinterpret_cast<CBMPTimer*>(lpParam);
	return pThis->ThreadProc(lpParam);
}
DWORD CBMPTimer::ThreadProc(LPVOID lpParam)
{	
	while (timeractive)
	{
	//MessageBox(0, L"In ThreadProc", L"", MB_OK);
		Sleep(ms_interval);
		CBMPWriter bmpW(hwTarget, ++ticks_so_far);
		bmpW.SaveBMP();
		bmpW.~CBMPWriter();
	}
	//ExitThread(0x0);
	return EXIT_SUCCESS;
}

void CBMPTimer::StopTimer()
{
	timeractive = false;
}