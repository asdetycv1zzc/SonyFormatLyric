#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<codecvt>
#include<algorithm>
#include"LyricHelper.h"
#include"UnicodeToUTF8.h"
using namespace std;

const string UTF8ToGB(const char* str)
{
	string result;
	WCHAR* strSrc;
	LPSTR szRes;

	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}

const wstring LyricHelper::_s_ReadLyric(const wstring& _k_LyricAddress)
{
	auto _LyricEncoding = _s_GetLyricEncoding(_k_LyricAddress);
	if (_LyricEncoding == TextEncoding::UnicodeBigEndian) return L"";
	FILE* _LyricFile = NULL;
	auto err = _wfopen_s(&_LyricFile, _k_LyricAddress.c_str(), L"rb+");

	if (_LyricFile == NULL) return L"";
	try
	{
		fseek(_LyricFile, 0, SEEK_END);
		size_t size = ftell(_LyricFile);
		fseek(_LyricFile, 0, SEEK_SET);
		std::string src;
		std::wstring _result;
		switch (_LyricEncoding)
		{
		case TextEncoding::UTF8WithBOM:
		{
			// UTF-8 file should offset 3 byte from start position.
			fseek(_LyricFile, sizeof(char) * 3, 0);

		}
		case TextEncoding::UTF8WithoutBOM:
		{
			// UTF-8 file without BOM should offset 0 byte from start position.
			fseek(_LyricFile, 0, 0);
		}
		}
		int buferSize = (int)size;
		char* szBuf = new char[buferSize + 1];
		memset(szBuf, 0, sizeof(char) * (buferSize + 1));
		fread(szBuf, sizeof(char), buferSize, _LyricFile);
		src.append(szBuf);
		delete[] szBuf;

		WCHAR* strSrc = NULL;

		switch (_LyricEncoding)
		{
		case TextEncoding::UTF8WithBOM:
		{
			[[fallthrough]];
		}
		case TextEncoding::UTF8WithoutBOM:
		{
			int i = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, NULL, 0);
			strSrc = new WCHAR[i + 1];
			MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, strSrc, i);
			break;
		}
		case TextEncoding::ANSI:
		{
			int i = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
			strSrc = new WCHAR[i + 1];
			MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, strSrc, i);
			break;
		}
		case TextEncoding::UNKNOWN:
		{
			return L"";
		}
		}
		_result = strSrc;
		delete[] strSrc;
		fclose(_LyricFile);
		//auto _result = wstring(result.begin(), result.end());
		return _result;
	}
	catch (const exception& e)
	{
		cerr << e.what() << endl;
		if (_LyricFile != NULL)
			fclose(_LyricFile);
		return L"";
	}
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

const TextEncoding LyricHelper::_s_GetLyricEncoding(const wstring& _k_LyricAddress)
{
	ifstream _LyricFile(_k_LyricAddress, ios::in | ios::binary);
	TextEncoding _result = TextEncoding::UNKNOWN;
	try
	{
		string _temp;
		unsigned char s2;
		_LyricFile.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
		int p = s2 << 8;
		_LyricFile.read((char*)&s2, sizeof(s2));//读取第二个字节
		p |= s2;

		auto _size = _s_GetLyricLength(_k_LyricAddress);

		_temp.clear();
		_temp.resize(_size, 0);

		switch (p)//判断文本前两个字节
		{
		case 0xfffe:	//65534
			_result = TextEncoding::Unicode;
			break;
		case 0xfeff:	//65279
			_result = TextEncoding::UnicodeBigEndian;
			break;
		case 0xefbb:	//61371
			_result = TextEncoding::UTF8WithBOM;
			break;
		default:
		{
			_LyricFile.seekg(0, ios::beg);
			for (size_t i = 0; i < _size; i++)
				_LyricFile.read(&_temp[i], sizeof(char));
			if (IsUTF8WithoutBOM(const_cast<char*>(_temp.c_str()), _temp.size()))
			{
				_result = TextEncoding::UTF8WithoutBOM;
				break;
			}
			else
			{
				_result = TextEncoding::ANSI;
				break;
			}
		}
		}
		_LyricFile.close();
		return _result;
	}
	catch (const exception& e)
	{
		cerr << e.what() << endl;
		if (_LyricFile.is_open())
			_LyricFile.close();
		return _result;
	}

}
const TextEncoding LyricHelper::_GetLyricEncoding()
{
	return _s_GetLyricEncoding(LyricHelper::LyricAddress);
}
/*https ://blog.csdn.net/kikityan/article/details/89923808*/

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
	while (_part_milisecond.size() >= 3)
		_part_milisecond.pop_back();
	while (_part_minute.size() < 2)
	{
		_part_minute.push_back(L'0');
		reverse(_part_minute.begin(), _part_minute.end());
	}
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

const ConvertionStatus LyricHelper::_s_WriteLyric(const wstring& _k_LyricAddress, const wstring& _k_CombinedLyric)
{
	FILE* _LyricFile = NULL;
	try
	{
		//unsigned long long i = 0;
		_wfopen_s(&_LyricFile, _k_LyricAddress.c_str(), L"wb");
		fseek(_LyricFile, 0, 0);
		if (_LyricFile == NULL) return ConvertionStatus::OpenFileFailed;

		auto _size = _k_CombinedLyric.size();
		//char* UTF8Content = UnicodeToUtf8(_k_CombinedLyric.c_str());

		//UTF8Content = szRes;
		/*
		LPSTR szRes;
		i = WideCharToMultiByte(CP_ACP, 0, _k_CombinedLyric.c_str(), -1, NULL, 0, NULL, NULL);
		szRes = new CHAR[i + 1];
		WideCharToMultiByte(CP_ACP, 0, _k_CombinedLyric.c_str(), -1, szRes, i, NULL, NULL);
		*/
		string UTF8source;
		WCHAR* strSrc;
		LPSTR szRes;

		int i = WideCharToMultiByte(CP_UTF8, 0, _k_CombinedLyric.c_str(), -1, NULL, 0, NULL, NULL);
		szRes = new CHAR[i + 1];
		WideCharToMultiByte(CP_UTF8, 0, _k_CombinedLyric.c_str(), -1, szRes, i, NULL, NULL);

		UTF8source = szRes;

		fwrite(UTF8source.c_str(), sizeof(char), strlen(UTF8source.c_str()), _LyricFile);

		fclose(_LyricFile);

		return ConvertionStatus::Success;
	}
	catch (const exception& e)
	{
		wcerr << e.what() << endl;
		if (_LyricFile != NULL)
			fclose(_LyricFile);
		return ConvertionStatus::UndefinedError;
	}
}
const ConvertionStatus LyricHelper::_WriteLyric(const wstring& _k_CombinedLyric)
{
	return _s_WriteLyric(LyricHelper::LyricAddress, _k_CombinedLyric);
}

const ConvertionStatus LyricHelper::s_ConvertLyric(const wstring& k_LyricAddress, const LinebreakType& k_LinebreakType)
{
	auto _sourceLyric = _s_ReadLyric(k_LyricAddress);
	if (!_s_BackupLyric(k_LyricAddress))return ConvertionStatus::BackupFailed;
	if (_sourceLyric.find(L'[') == wstring::npos || _sourceLyric.find(L']') == wstring::npos) return ConvertionStatus::NotLyricFormat;
	auto _splitedLyric = _s_SplitLyric(_sourceLyric);
	auto _size = _splitedLyric.size();
	wstring _tempLyricContent, _tempConvertedTime;
	vector<wstring> _tempAllLyricLines;
	for (size_t i = 0; i < _size; i++)
	{
		_tempLyricContent = _s_GetLyricContent(_splitedLyric[i]);
		if (_tempLyricContent == L"") continue;
		_tempConvertedTime = _s_ConvertTime(_s_GetLyricTime(_splitedLyric[i]));
		_tempAllLyricLines.push_back(_s_CombineLyricLine(_tempConvertedTime, _tempLyricContent));
	}
	auto _tempAllLyric = _s_CombineAllLyric(_tempAllLyricLines, k_LinebreakType);
	auto _result = _s_WriteLyric(k_LyricAddress, _tempAllLyric);
	if (_result) return ConvertionStatus::Success;
	return ConvertionStatus::UndefinedError;
}
const ConvertionStatus LyricHelper::ConvertLyric(const LinebreakType& k_LinebreakType)
{
	auto _status = s_ConvertLyric(LyricHelper::LyricAddress, k_LinebreakType);
	Status = _status;
	return Status;
}

const ConvertionStatus LyricHelper::GetConvertionStatus()
{
	return Status;
}

const wstring LyricHelper::GetLyricShortAddress()
{
	auto _shortAddress = this->LyricAddress.substr(this->LyricAddress.find_last_of(L"\\") + 1);
	return _shortAddress;
}

LyricHelper::LyricHelper()
{
	this->LyricAddress = L"";
	this->Status = ConvertionStatus::UndefinedError;
}
LyricHelper::LyricHelper(const wstring& k_LyricAddress, const LinebreakType& k_LinebreakType)
{
	this->LyricAddress.clear();
	this->LyricAddress += wstring(k_LyricAddress);
	this->Status = ConvertionStatus::UndefinedError;
}
LyricHelper::~LyricHelper()
{
	this->LyricAddress.clear();
}