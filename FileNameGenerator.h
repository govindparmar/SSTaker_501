#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class CFileNameGenerator  
{
private:
	string next_filename;
	int count;
public:
	string GetNextFileName();
	CFileNameGenerator();
	~CFileNameGenerator();

};