#pragma once
#include <Windows.h>
#include "BMPWriter.h"
/**
 * BMPTimer class - a timer for periodically saving screenshots of a specified window.
 */
class CBMPTimer
{
private:
	int ticks_so_far; // New variable added with refactor #3 - keeps track of ticks made by our timer
	int ms_interval; // Inteval between clicks
	HWND hwTarget; // Target window
	HANDLE hThread; // Thread for timer tick
	LPVOID thistimer; // Pointer to "this" class instance
	static bool timeractive; // Flag for exitting the timer thread
public:
	// Stub that invokes threadproc (required for using a callback function belonging to a class)
	static DWORD StaticThreadProc(LPVOID);
	// Actual thread procedure
	DWORD ThreadProc(LPVOID);
	// Sets the exit flag condition for the thread proc
	void StopTimer();
	// Constructor/deconstructor
	CBMPTimer(HWND, int);
	~CBMPTimer();
	friend class UnitTest1;
};
