#include "FileNameGenerator.h"

CFileNameGenerator::CFileNameGenerator()
{
	count = 1;
}

CFileNameGenerator::~CFileNameGenerator()
{

}


std::string CFileNameGenerator::GetNextFileName()
{
	int temp = count;
	int num_digits = log10(temp)+1;
	//if(num_digits<=0) num_digits=1;
	char *tmp=new char[num_digits+1];
	for(int i = 0; i < num_digits; i++)
	{
		tmp[num_digits-i-1]=(temp%10)+'0';
		temp/=10;
	}
	tmp[num_digits]=0;
	string countstr = tmp;
	delete tmp;
	next_filename="shot_";
	next_filename+=countstr;
	next_filename+=".bmp";
	count++;
	return next_filename;
}