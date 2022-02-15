#include<iostream>
#include<string>
#include<cstring>
#include<Windows.h>

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

bool IsUTF8WithoutBOM(char* _data, int size)
{
	char *data = new char[size];
	memset(data, 0, size);
	memcpy(data, _data, size);

	int encodingBytesCount = 0;
	bool allTextsAreASCIIChars = true;


	for (int i = 0; i < size; i++)
	{
		char current = data[i];
		//判断是否都是ASCII字符，当包括中文字符时allTextsAreASCIIChars为false
		if ((current & 0x80) == 0x80)
			allTextsAreASCIIChars = false;

		//判断是否为一个字符的开始字节
		if (encodingBytesCount == 0)
		{
			if ((current & 0x80) == 0)
			{
				// ASCII chars, from 0x00-0x7F
				continue;
			}

			if ((current & 0xC0) == 0xC0)	///大于等于两字节
			{
				encodingBytesCount = 1;
				current <<= 2;

				// More than two bytes used to encoding a unicode char.
				// Calculate the real length.
				while ((current & 0x80) == 0x80)
				{
					current <<= 1;	//判断下一位是否为1
					encodingBytesCount++;	//当前字符编码字节数
				}
			}
			else
			{
				// Invalid bits structure for UTF8 encoding rule.
				return false;
			}
		}
		else
		{
			// Following bytes, must start with 10.
			if ((current & 0xC0) == 0x80)	///当前字节是否以10开头
			{
				encodingBytesCount--;
			}
			else
			{
				return false;
			}
		}
	}
	if (encodingBytesCount != 0)
	{
		// Invalid bits structure for UTF8 encoding rule.
		// Wrong following bytes count.
		return false;
	}
	return !allTextsAreASCIIChars;
}
/*https ://blog.csdn.net/j_oop/article/details/105893664*/