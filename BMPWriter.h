#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
using namespace std;
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
	int count;
	int xd, yd;
public:
	string NextFileName();
	void InitFH();
	void InitIH();
	void InitInfo();	
	CBMPWriter(HWND, int);
	~CBMPWriter();
	void SaveBMP();
};

