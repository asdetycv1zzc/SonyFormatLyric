#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

enum LinebreakType
{
	UNIX,	/* \n */
	WINDOWS,/* \r\n */
};

class LyricHelper
{
private:
	wstring LyricAddress;

	const static wstring _s_ReadLyric(const wstring& _k_LyricAddress);
	const static unsigned long long _s_GetLyricLength(const wstring& _k_LyricAddress);
	const wstring _ReadLyric();
	const unsigned long long _GetLyricLength();

	const static bool _s_BackupLyric(const wstring& _k_LyricAddress);
	const bool _BackupLyric();

	const static vector<wstring> _s_SplitLyric(const wstring& _k_SourceLyric);
	const vector<wstring> _SplitLyric();

	const static wstring _s_GetLyricContent(const wstring& _k_SourceLyricLine);
	const static wstring _s_GetLyricTime(const wstring& _k_SourceLyricLine);

	const static wstring _s_ConvertTime(const wstring& _k_SourceTime);

	const static wstring _s_CombineLyricLine(const wstring& _k_ConvertedTime, const wstring& _k_LyricLine);
	const static wstring _s_CombineAllLyric(const vector<wstring> &_k_LyricLines, const LinebreakType &_k_LinebreakType = LinebreakType::UNIX);

	const static bool _s_WriteLyric(const wstring& _k_LyricAddress, const wstring& _k_CombinedLyric);
	const bool _WriteLyric(const wstring& _k_CombinedLyric);

public:

	LyricHelper();
	LyricHelper(const wstring& k_LyricAddress, const LinebreakType& k_LinebreakType = LinebreakType::UNIX);
	~LyricHelper();

	//static wstring s_ReadLyric(const wstring& k_LyricAddress);
	//wstring ReadLyric();

	//static bool s_BackupLyric(const wstring& k_LyricAddress);
	//bool BackupLyric();

	//static vector<wstring> s_SplitLyric(const wstring& k_LyricAddress);
	//vector<wstring> SplitLyric();

	//static wstring s_GetLyricContent(const wstring& k_SourceLyricLine);
	//static wstring s_GetLyricTime(const wstring& k_SourceLyricLine);

	//static wstring s_ConvertTime(const wstring& k_SourceTime);
	//

	//static wstring s_CombineLyricLine(const wstring& k_ConvertedTime, const wstring& k_LyricLine);
	//static wstring s_CombineAllLyric(const vector<wstring>& k_LyricLines, const LinebreakType& k_LinebreakType = LinebreakType::UNIX);

	//static wstring s_WriteLyric(const wstring& k_LyricAddress);
	//wstring WriteLyric();

	const static bool s_ConvertLyric(const LinebreakType& k_LinebreakType = LinebreakType::UNIX);
	const bool ConvertLyric();

};

const wstring LyricHelper::_s_ReadLyric(const wstring& _k_LyricAddress)
{
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

		while (_LyricFile.read((char *)&_TempContent[i], sizeof(wchar_t)))
		{
			_Content[i] = _TempContent[i];
			i++;
		}
		return _Content;
	}
	catch (const exception &e)
	{
		cerr << e.what() << endl;
		if (_LyricFile.is_open())
			_LyricFile.close();
		return NULL;
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
	catch (const exception &e)
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
	wstring _part_minute,_part_second,_part_milisecond;
	_part_minute = _k_SourceTime.substr(0, _k_SourceTime.find_first_of(L':'));
	_part_second = _k_SourceTime.substr(_k_SourceTime.find_first_of(L':') + 1, _k_SourceTime.find_first_of(L'.') - _k_SourceTime.find_first_of(L':') - 1);
	_part_milisecond = _k_SourceTime.substr(_k_SourceTime.find_first_of(L'.') + 1);
}