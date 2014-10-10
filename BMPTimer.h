#include <Windows.h>
#include "BMPWriter.h"
#include "FileNameGenerator.h"
class CBMPTimer
{
private:
	int ms_interval;
	HWND hwTarget;
	CFileNameGenerator NameG;
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
