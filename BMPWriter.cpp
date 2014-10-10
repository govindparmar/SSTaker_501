#define _CRT_SECURE_NO_WARNINGS
#include "BMPWriter.h"

/** Initalizes the fields in the bitmap file header - file signature, header size, etc... */
void CBMPWriter::InitFH()
{
	bfHeader.bfType = 0x4d42; 
	bfHeader.bfSize = 0;
	bfHeader.bfReserved1 = 0;
	bfHeader.bfReserved2 = 0;
	bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
}

/** Initalizes the bitmap info header structure - bit intensity, planes, dimensions etc... */
void CBMPWriter::InitIH()
{
	biHeader.biSize = sizeof(biHeader);
	biHeader.biBitCount = 24;
	biHeader.biCompression = BI_RGB;
	biHeader.biHeight = yd;
	biHeader.biWidth = xd;
	biHeader.biPlanes = 1;
	biHeader.biSizeImage = 0;
	biHeader.biXPelsPerMeter = 0;
	biHeader.biYPelsPerMeter = 0;
	biHeader.biClrImportant = 0;
	biHeader.biClrUsed = 0;
}

void CBMPWriter::InitInfo()
{
	bInfo.bmiHeader = biHeader;
}

CBMPWriter::CBMPWriter(HWND window, const char *filename)
{
	// Initialize the bitmap file headers
	RECT r;
	hwTarget = window;
	strcpy(fname, filename);
	GetWindowRect(hwTarget, &r);
	int xd = r.right - r.left;
	int yd = r.bottom - r.top;
	
	InitFH();
	InitIH();
	InitInfo();

	len_data = ((((24 * xd + 31)&(~31)) / 8)*yd);

	data = new char[len_data];
	windowDC = GetWindowDC(hwTarget);
	tempDC = CreateCompatibleDC(windowDC);
	hBitmap = CreateDIBSection(windowDC, &bInfo, DIB_RGB_COLORS, (void**)&data, 0, 0);
	SelectObject(tempDC, hBitmap);
	BitBlt(tempDC, 0, 0, xd, yd, windowDC, 0, 0, SRCCOPY);
}

CBMPWriter::~CBMPWriter()
{
	DeleteDC(tempDC);
	ReleaseDC(hwTarget, windowDC);
	DeleteObject(hBitmap);
}

void CBMPWriter::SaveBMP()
{
	HANDLE hFile = CreateFileA(fname, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile)
	{
		DWORD dwWritten = 0;
		WriteFile(hFile, &bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
		WriteFile(hFile, &biHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
		WriteFile(hFile, data, len_data, &dwWritten, NULL);
	}
	else
	{
		MessageBox(0, _T("Could not open the file."), _T("Error"), MB_OK | MB_ICONSTOP);
	}
	CloseHandle(hFile);
}