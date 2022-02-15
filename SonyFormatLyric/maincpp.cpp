#include<iostream>
#include<string>
#include<Windows.h>
#include"LyricHelper.h"
using namespace std;


std::wstring stringToWstring(const std::string& str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return std::wstring(L"");

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return std::wstring(L"");

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return wstr;
}
/*https ://blog.csdn.net/qicaiyuwu/article/details/89346871*/

int main(int argc, char* argv[])
{
	cout << "Argument Count: " << argc << endl;
	for (int i = 1; i < argc; i++)
	{
		string _temp = argv[i];
		wstring _wideAddress(stringToWstring(_temp));
		cout << _temp << endl;
		auto _cl = LyricHelper(_wideAddress);
		_cl.ConvertLyric();
	}
}