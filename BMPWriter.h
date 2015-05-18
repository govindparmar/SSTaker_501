#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
using namespace std;
/**
 * BMPWriter class - a file writer for bitmap screenshots of a given window.
 */
class CBMPWriter
{
private:
	char *data; // Stores the array of bits representing a bitmap of a window
	int len_data; // Length of data
	//char fname[MAX_PATH]; // Filename to write out
	BITMAPFILEHEADER bfHeader; // Bitmap file header structure
	BITMAPINFOHEADER biHeader; // Bitmap file header #2 structure
	BITMAPINFO bInfo; // Bitmap file header #3 structure
	HBITMAP hBitmap; // Handle to the bitmap object
	HWND hwTarget; // Target window
	HDC windowDC, tempDC; // Device context for target window and temporary copy in memory
	int count; // Refactor #3 - generate file names internally rather than use lazy class CFileNameGenerator
	int xd, yd; // x and y dimensions of bitmap
public:
	char fname[MAX_PATH]; // Filename to write out
	// Generate next file name of bitmap file - refactored out of CFileNameGenerator
	string NextFileName();
	// Initialization functions for the 3 bitmap file header/signature structs
	void InitFH();
	void InitIH();
	void InitInfo();	
	// Class constructor - calls the initialization functions
	CBMPWriter(HWND, int);
	// Deconstructor - cleans up GDI objects and frees memory
	~CBMPWriter();
	// Performs the actual bitmap file write
	void SaveBMP();
};

