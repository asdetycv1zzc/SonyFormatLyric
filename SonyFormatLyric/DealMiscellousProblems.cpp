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
		//�ж��Ƿ���ASCII�ַ��������������ַ�ʱallTextsAreASCIICharsΪfalse
		if ((current & 0x80) == 0x80)
			allTextsAreASCIIChars = false;

		//�ж��Ƿ�Ϊһ���ַ��Ŀ�ʼ�ֽ�
		if (encodingBytesCount == 0)
		{
			if ((current & 0x80) == 0)
			{
				// ASCII chars, from 0x00-0x7F
				continue;
			}

			if ((current & 0xC0) == 0xC0)	///���ڵ������ֽ�
			{
				encodingBytesCount = 1;
				current <<= 2;

				// More than two bytes used to encoding a unicode char.
				// Calculate the real length.
				while ((current & 0x80) == 0x80)
				{
					current <<= 1;	//�ж���һλ�Ƿ�Ϊ1
					encodingBytesCount++;	//��ǰ�ַ������ֽ���
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
			if ((current & 0xC0) == 0x80)	///��ǰ�ֽ��Ƿ���10��ͷ
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