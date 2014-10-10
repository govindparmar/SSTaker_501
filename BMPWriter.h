#pragma once
#include <Windows.h>
#include <tchar.h>
class CBMPWriter
{
private:
	char *data;
	int len_data;
	char fname[MAX_PATH];
	BITMAPFILEHEADER bfHeader;
	BITMAPINFOHEADER biHeader;
	BITMAPINFO bInfo;
	HBITMAP hBitmap;
	HWND hwTarget;
	HDC windowDC, tempDC;


public:
	CBMPWriter(HWND, const char*);
	~CBMPWriter();
	void SaveBMP();
};

