#include<iostream>
#include<string>
#include<Windows.h>
#include"LyricHelper.h"
#include"DealMiscellousProblems.h"
using namespace std;

int wmain(int argc, wchar_t* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		cout << "Lyrics Count: " << argc - 1 << endl;
		wstring _wideAddress(argv[i]);
		wprintf(L"%s", _wideAddress.c_str());
		auto _cl = LyricHelper(_wideAddress);
		_cl.ConvertLyric();
	}
	return 0;
}