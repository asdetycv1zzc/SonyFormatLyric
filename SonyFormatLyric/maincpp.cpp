#include<iostream>
#include<string>
#include<Windows.h>
#include"LyricHelper.h"
#include"DealMiscellousProblems.h"
using namespace std;

int wmain(int argc, wchar_t* argv[])
{
	cout << "Lyrics Count: " << argc << endl;
	//wstring _temp = L"Kawin - a fool.lrc";
	//wstring _wideAddress(stringToWstring(_temp));
	for (int i = 1; i < argc; i++)
	{
		//string _temp = DeleteChineseBlankSpace(const_cast<char*>("Kawin - a fool.lrc"));
		//wstring _wideAddress(stringToWstring(_temp));
		wstring _wideAddress(argv[i]);
		wprintf(L"%s", _wideAddress.c_str());
		auto _cl = LyricHelper(_wideAddress);
		_cl.ConvertLyric();
	}
	return 0;
}