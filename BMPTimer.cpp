#include "BMPTimer.h"
bool CBMPTimer::timeractive;
/**
 * Class constructor for CBMPTimer
 * Takes a handle to the target window for screenshotting
 *  and an interval in milliseconds.
 */
CBMPTimer::CBMPTimer(HWND hWnd, int ms)
{
	// Assign member variables
	hwTarget = hWnd;
	ms_interval = ms;
	// This cast is necessary so that the ThreadProc function can be called from a static stub
	thistimer = reinterpret_cast<LPVOID>(this);
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StaticThreadProc, thistimer, 0, NULL);
	// Set other member variables...
	timeractive = true;
	ticks_so_far=0;
}
/**
 * Class deconstructor for CBMPTimer
 * Frees dynamically allocated memory
 */
CBMPTimer::~CBMPTimer()
{
	delete thistimer;
}
// Stub method that invokes ThreadProc.
// Necessary so that ThreadProc can belong to the class and also be specified as a callback function.
DWORD CBMPTimer::StaticThreadProc(LPVOID lpParam)
{
	CBMPTimer *pThis = reinterpret_cast<CBMPTimer*>(lpParam);
	return pThis->ThreadProc(lpParam);
}
// Actual thread procedure.
// Uses a CBMPWriter object to save a bitmap of the target window as long as the flag to keep running stays on.
DWORD CBMPTimer::ThreadProc(LPVOID lpParam)
{	
	while (timeractive)
	{
		Sleep(ms_interval);
		CBMPWriter bmpW(hwTarget, ++ticks_so_far);
		bmpW.SaveBMP();
		bmpW.~CBMPWriter();
	}
	return EXIT_SUCCESS;
}
// Method to set the thread flag off and quit the screenshot taking.
void CBMPTimer::StopTimer()
{
	timeractive = false;
}