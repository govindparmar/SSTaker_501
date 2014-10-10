#include <Windows.h>
#include "BMPWriter.h"
class CBMPTimer
{
private:
	int ticks_so_far; // New variable added with refactor #3 - keeps track of ticks made by our timer
	int ms_interval;
	HWND hwTarget;
	HANDLE hThread;
	LPVOID thistimer;
	static bool timeractive;
public:
	static DWORD StaticThreadProc(LPVOID);
	DWORD ThreadProc(LPVOID);
	void StopTimer();
	CBMPTimer(HWND, int);
	~CBMPTimer();
};
