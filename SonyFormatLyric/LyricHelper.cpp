#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include"LyricHelper.h"
#include"UnicodeToUTF8.h"
using namespace std;

const wstring LyricHelper::_s_ReadLyric(const wstring& _k_LyricAddress)
{
	/*
	fstream _LyricFile;
	try
	{
		unsigned long long i = 0;

		_LyricFile.open(_k_LyricAddress, ios::in | ios::binary);
		_LyricFile.seekg(0, ios::beg);

		if (!_LyricFile.is_open()) return NULL;

		auto _size = _s_GetLyricLength(_k_LyricAddress);

		vector<wchar_t> _TempContent(_size, 0);
		wstring _Content;

		_Content.resize(_size, 0);

		while (_LyricFile.read((char*)&_TempContent[i], sizeof(wchar_t)))
		{
			_Content[i] = _TempContent[i];
			i++;
		}
		return _Content;
	}
	catch (const exception& e)
	{
		cerr << e.what() << endl;
		if (_LyricFile.is_open())
			_LyricFile.close();
		return NULL;
	}
	*/
	FILE* fp = NULL;
	fp = fopen(UnicodeToUtf8(_k_LyricAddress.c_str()), "rb");
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::string result;
	std::wstring _result;

	if (fp != NULL)
	{
		// UTF-8 file should offset 3 byte from start position.
		fseek(fp, 0, 0);
		int buferSize = (int)size - 3;
		char* szBuf = new char[buferSize + 1];
		memset(szBuf, 0, sizeof(char) * (buferSize + 1));
		fread(szBuf, sizeof(char), buferSize - 3, fp);
		result.append(szBuf);
		delete[] szBuf;

		WCHAR* strSrc;

		int i = MultiByteToWideChar(CP_UTF8, 0, result.c_str(), -1, NULL, 0);
		strSrc = new WCHAR[i + 1];
		MultiByteToWideChar(CP_UTF8, 0, result.c_str(), -1, strSrc, i);

		_result = strSrc;

		delete[] strSrc;
	}

	fclose(fp);
	//auto _result = wstring(result.begin(), result.end());
	return _result;
}
const unsigned long long LyricHelper::_s_GetLyricLength(const wstring& _k_LyricAddress)
{

	fstream _LyricFile;
	try
	{
		_LyricFile.open(_k_LyricAddress, ios::in | ios::binary);
		auto OriginLocation = _LyricFile.tellg();
		_LyricFile.seekg(0, ios::end);
		auto Length = _LyricFile.tellg();
		_LyricFile.seekg(OriginLocation, ios::beg);
		return Length;
	}
	catch (const exception& e)
	{
		cerr << e.what() << endl;
		if (_LyricFile.is_open())
			_LyricFile.close();
		return -1;
	}
}
const wstring LyricHelper::_ReadLyric()
{
	return _s_ReadLyric(LyricHelper::LyricAddress);
}
const unsigned long long LyricHelper::_GetLyricLength()
{
	return _s_GetLyricLength(LyricHelper::LyricAddress);
}

const bool LyricHelper::_s_BackupLyric(const wstring& _k_LyricAddress)
{
	auto _source = _s_ReadLyric(_k_LyricAddress);
	auto _destAddress = _k_LyricAddress + wstring(L".bak");
	auto _result = _s_WriteLyric(_destAddress, _source);
	return _result;
}
const bool LyricHelper::_BackupLyric()
{
	return _s_BackupLyric(LyricHelper::LyricAddress);
}

const vector<wstring> LyricHelper::_s_SplitLyric(const wstring& _k_SourceLyric)
{
	wstring _temp(_k_SourceLyric), buffer;
	vector<wstring> _result;

	auto _size = _temp.size();

	for (unsigned long long i = 0; i < _size; i++)
	{
		if (_temp[i] != L'\r' && _temp[i] != L'\n')
		{
			buffer.push_back(_temp[i]);
			continue;
		}
		if (_temp[i] == L'\r' || _temp[i] == L'\n')
		{
			if (buffer.size() > 0)
			{
				_result.push_back(buffer);
				buffer.clear();
			}
			else
				_result.push_back(L"\n");
			i += 1;
		}
	}
	return _result;
}
const vector<wstring> LyricHelper::_SplitLyric()
{
	auto _source = _s_ReadLyric(LyricHelper::LyricAddress);
	return _s_SplitLyric(_source);
}

const wstring LyricHelper::_s_GetLyricContent(const wstring& _k_SourceLyricLine)
{
	auto _beginPos = _k_SourceLyricLine.find_first_of(L']') + 1;	//Jump one wchar_t in order to skip the "]" flag
	auto _LyricLength = _k_SourceLyricLine.size() - _beginPos;
	auto _result = wstring(_k_SourceLyricLine).substr(_beginPos, _LyricLength);
	return _result;
}
const wstring LyricHelper::_s_GetLyricTime(const wstring& _k_SourceLyricLine)
{
	auto _beginPos = 1;
	auto _TimeTextLength = _k_SourceLyricLine.find_first_of(L']') - 1; //Guess how did I get the magic number lol
	auto _result = wstring(_k_SourceLyricLine).substr(_beginPos, _TimeTextLength);
	return _result;
}

const wstring LyricHelper::_s_ConvertTime(const wstring& _k_SourceTime)
{
	wstring _part_minute, _part_second, _part_milisecond, _result;
	_part_minute = _k_SourceTime.substr(0, _k_SourceTime.find_first_of(L':'));
	_part_second = _k_SourceTime.substr(_k_SourceTime.find_first_of(L':') + 1, _k_SourceTime.find_first_of(L'.') - _k_SourceTime.find_first_of(L':') - 1);
	_part_milisecond = _k_SourceTime.substr(_k_SourceTime.find_first_of(L'.') + 1);
	while (_part_milisecond.size() > 3)
		_part_milisecond.pop_back();
	_result = L'[' + _part_minute + L':' + _part_second + L'.' + _part_milisecond + L']';
	return _result;
}

const wstring LyricHelper::_s_CombineLyricLine(const wstring& _k_ConvertedTime, const wstring& _k_LyricLine)
{
	auto _result = _k_ConvertedTime + _k_LyricLine;
	return _result;
}
const wstring LyricHelper::_s_CombineAllLyric(const vector<wstring>& _k_LyricLines, const LinebreakType& _k_LinebreakType)
{
	auto _size = _k_LyricLines.size();
	wstring _result;
	for (size_t i = 0; i < _size; i++)
	{
		_result += _k_LyricLines[i];
		switch (_k_LinebreakType)
		{
		case LinebreakType::WINDOWS:
		{
			_result += L"\r";
			[[fallthrough]];
		}
		case LinebreakType::UNIX:
		{
			_result += L"\n";
			break;
		}
		}
	}
	return _result;
}

const bool LyricHelper::_s_WriteLyric(const wstring& _k_LyricAddress, const wstring& _k_CombinedLyric)
{
	FILE* _LyricFile = NULL;
	try
	{
		//unsigned long long i = 0;
		_LyricFile = fopen(UnicodeToUtf8(_k_LyricAddress.c_str()), "w");
		fseek(_LyricFile, 0 , 0);
		if (_LyricFile == NULL) return NULL;

		auto _size = _k_CombinedLyric.size();
		//char* UTF8Content = UnicodeToUtf8(_k_CombinedLyric.c_str());

		//UTF8Content = szRes;
		/*
		LPSTR szRes;
		i = WideCharToMultiByte(CP_ACP, 0, _k_CombinedLyric.c_str(), -1, NULL, 0, NULL, NULL);
		szRes = new CHAR[i + 1];
		WideCharToMultiByte(CP_ACP, 0, _k_CombinedLyric.c_str(), -1, szRes, i, NULL, NULL);
		*/
		string UTF8;
		WCHAR* strSrc;
		LPSTR szRes;

		int i = WideCharToMultiByte(CP_ACP, 0, _k_CombinedLyric.c_str(), -1, NULL, 0, NULL, NULL);
		szRes = new CHAR[i + 1];
		WideCharToMultiByte(CP_ACP, 0, _k_CombinedLyric.c_str(), -1, szRes, i, NULL, NULL);

		UTF8 = szRes;
		
		fprintf(_LyricFile, "%s", UTF8.c_str());
		fclose(_LyricFile);

		return true;
	}
	catch (const exception& e)
	{
		cerr << e.what() << endl;
		if (_LyricFile != NULL)
			fclose(_LyricFile);
		return false;
	}
}
const bool LyricHelper::_WriteLyric(const wstring& _k_CombinedLyric)
{
	return _s_WriteLyric(LyricHelper::LyricAddress, _k_CombinedLyric);
}

const bool LyricHelper::s_ConvertLyric(const wstring& k_LyricAddress, const LinebreakType& k_LinebreakType)
{
	auto _sourceLyric = _s_ReadLyric(k_LyricAddress);
	if (!_s_BackupLyric(k_LyricAddress)) return false;
	auto _splitedLyric = _s_SplitLyric(_sourceLyric);
	auto _size = _splitedLyric.size();
	wstring _tempLyricContent, _tempConvertedTime;
	vector<wstring> _tempAllLyricLines;
	for (size_t i = 0; i < _size; i++)
	{
		_tempLyricContent = _s_GetLyricContent(_splitedLyric[i]);
		_tempConvertedTime = _s_ConvertTime(_s_GetLyricTime(_splitedLyric[i]));
		_tempAllLyricLines.push_back(_s_CombineLyricLine(_tempConvertedTime, _tempLyricContent));
	}
	auto _tempAllLyric = _s_CombineAllLyric(_tempAllLyricLines, k_LinebreakType);
	auto _result = _s_WriteLyric(k_LyricAddress, _tempAllLyric);
	return _result;
}
const bool LyricHelper::ConvertLyric(const LinebreakType& k_LinebreakType)
{
	return s_ConvertLyric(LyricHelper::LyricAddress, k_LinebreakType);
}

LyricHelper::LyricHelper()
{
	this->LyricAddress = L"";
}
LyricHelper::LyricHelper(const wstring& k_LyricAddress, const LinebreakType& k_LinebreakType)
{
	this->LyricAddress.clear();
	this->LyricAddress += wstring(k_LyricAddress);
}
LyricHelper::~LyricHelper()
{
	this->LyricAddress.clear();
}