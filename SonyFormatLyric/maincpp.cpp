#include<iostream>
#include<string>
#include<Windows.h>
#include"LyricHelper.h"
#include"DealMiscellousProblems.h"
using namespace std;

int wmain(int argc, wchar_t* argv[])
{
	if (argc <= 1)
	{
		wchar_t _temp[3001] = { 0 };
		wcin.imbue(locale("chs"));
		wcin.getline(_temp, 3001);
		wstring _wideAddress(_temp);
		wprintf(L"%s", _wideAddress.c_str());
		auto _cl = LyricHelper(_wideAddress);
		_cl.ConvertLyric();
	}
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