#include<iostream>
#include<string>
#include"LyricHelper.h"
using namespace std;

int main(int argc, char* argv[])
{
	cout << "Argument Count: " << argc << endl;
	auto _cl = LyricHelper(L"J:\\1.lrc");
	_cl.ConvertLyric();
	for (int i = 1; i < argc; i++)
	{
		string _temp = argv[i];
		wstring _wideAddress(_temp.begin(), _temp.end());
		auto _cl = LyricHelper(_wideAddress);
		_cl.ConvertLyric();
	}
}