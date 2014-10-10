#include "BMPTimer.h"
bool CBMPTimer::timeractive;

CBMPTimer::CBMPTimer(HWND hWnd, int ms)
{
	hwTarget = hWnd;
	ms_interval = ms;
	thistimer = reinterpret_cast<LPVOID>(this);
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StaticThreadProc, thistimer, 0, NULL);
	timeractive = true;
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
	int i = 0;
	while (timeractive)
	{
	//MessageBox(0, L"In ThreadProc", L"", MB_OK);
		Sleep(ms_interval);
		CBMPWriter bmpW(hwTarget, NameG.GetNextFileName().c_str());
		bmpW.SaveBMP();
		bmpW.~CBMPWriter();
		i++;
	}
	//ExitThread(0x0);
	return EXIT_SUCCESS;
}

void CBMPTimer::StopTimer()
{
	timeractive = false;
}