#define _CRT_SECURE_NO_WARNINGS
#include "BMPWriter.h"

// Generates the next filename to write out the bitmap data to.
string CBMPWriter::NextFileName()
{
	string next_filename;
	int temp = count;
	// Get the number of digits in the count variable by using logarithm base 10
	int num_digits = (int)log10(temp)+1;
	char tmp[32];
	// Write out the ascii value of the count into the string
	for(int i = 0; i < num_digits; i++)
	{
		tmp[num_digits-i-1]=(temp%10)+'0'; // digits 0-9 plus ascii '0' = char representation
		temp/=10; // divide by 10 to process next place-value
	}
	tmp[num_digits]=0; // terminate string with null char
	// create final filename string and return it.
	string countstr = tmp;
	next_filename="shot_";
	next_filename+=countstr;
	next_filename+=".bmp";
	return next_filename;
}

/** Initalizes the fields in the bitmap file header - file signature, header size, etc... */
void CBMPWriter::InitFH()
{
	bfHeader.bfType = 0x4d42;  // magic number- identifies the file as a valid bitmap to image viewers etc.
	bfHeader.bfSize = 0;     // This and other 3 are reserved and always 0
	bfHeader.bfReserved1 = 0;
	bfHeader.bfReserved2 = 0;
	bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);  // Specifies the number of bytes occupied by the headers (before the actual array of bits/pixels begins)
}

/** Initalizes the bitmap info header structure - bit intensity, planes, dimensions etc... */
void CBMPWriter::InitIH()
{
	biHeader.biSize = sizeof(biHeader);
	biHeader.biBitCount = 24; // 24-bits = full intensity/lossless RGB image of window
	biHeader.biCompression = BI_RGB; // No compression
	biHeader.biHeight = yd; // dimensions
	biHeader.biWidth = xd;
	biHeader.biPlanes = 1; // number of planes
	biHeader.biSizeImage = 0; // all reserved fields below, must be 0
	biHeader.biXPelsPerMeter = 0;
	biHeader.biYPelsPerMeter = 0;
	biHeader.biClrImportant = 0;
	biHeader.biClrUsed = 0;
}
// Initializes the bInfo structure (not a file header but necessary for various bitmap WinAPI functions)
void CBMPWriter::InitInfo()
{
	bInfo.bmiHeader = biHeader;
}

CBMPWriter::CBMPWriter(HWND window, int tick)
{
	// Set local variables...
	RECT r;
	hwTarget = window;
	count = tick;
	// Get the file name to save for this instance of CBMPWriter
	strcpy(fname, NextFileName().c_str());
	// Get dimensions...
	GetWindowRect(hwTarget, &r);
	 xd = r.right - r.left;
	 yd = r.bottom - r.top;
	count=0;
	// Initialize file/bitmap headers and signatures etc
	InitFH();
	InitIH();
	InitInfo();

	// Calculate length of data.
	len_data = ((((24 * xd + 31)&(~31)) / 8)*yd);
	// Generate dynamic char (byte) array based on length.
	data = new char[len_data];
	// Get the device context of the target window
	windowDC = GetWindowDC(hwTarget);
	// Create a new device context *in memory* with identical properties as the window DC
	tempDC = CreateCompatibleDC(windowDC);
	// Get the bitmap of the window
	hBitmap = CreateDIBSection(windowDC, &bInfo, DIB_RGB_COLORS, (void**)&data, 0, 0);
	// Select it as the GDI object we want to use
	SelectObject(tempDC, hBitmap);
	// And copy it into the memory device context 
	BitBlt(tempDC, 0, 0, xd, yd, windowDC, 0, 0, SRCCOPY);
}

/**
 * Class deconstructor for CBMPWriter
 * Deletes GDI objects, frees dynamic memory, etc. to prevent memory leaks
 */
CBMPWriter::~CBMPWriter()
{
	DeleteDC(tempDC);
	ReleaseDC(hwTarget, windowDC);
	DeleteObject(hBitmap);
}

// Performs the actual write of the bitmap.
void CBMPWriter::SaveBMP()
{
	// Create a file (if we can)
	HANDLE hFile = CreateFileA(fname, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile)
	{
		// If we can, write out the headers, then write out the bitmap
		DWORD dwWritten = 0;
		WriteFile(hFile, &bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
		WriteFile(hFile, &biHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
		WriteFile(hFile, data, len_data, &dwWritten, NULL);
	}
	else // If not, tell the user, and exit the class function
	{
		MessageBox(0, _T("Could not open the file."), _T("Error"), MB_OK | MB_ICONSTOP);
	}
	// Free handle from memory.
	CloseHandle(hFile);
}