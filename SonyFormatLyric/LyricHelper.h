#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include"DealMiscellousProblems.h"

enum LinebreakType
{
	UNIX,	/* \n */
	WINDOWS,/* \r\n */
};

enum TextEncoding
{
	UTF8WithoutBOM,
	UTF8WithBOM,
	ANSI,
	Unicode,
	UnicodeBigEndian,
	GB2312,
	UNKNOWN,
};

class LyricHelper
{
private:
	std::wstring LyricAddress;

	const static std::wstring _s_ReadLyric(const std::wstring& _k_LyricAddress);
	const static unsigned long long _s_GetLyricLength(const std::wstring& _k_LyricAddress);
	const std::wstring _ReadLyric();
	const unsigned long long _GetLyricLength();

	const static TextEncoding _s_GetLyricEncoding(const std::wstring& _k_LyricAddress);
	const TextEncoding _GetLyricEncoding();

	const static bool _s_BackupLyric(const std::wstring& _k_LyricAddress);
	const bool _BackupLyric();

	const static std::vector<std::wstring> _s_SplitLyric(const std::wstring& _k_SourceLyric);
	const std::vector<std::wstring> _SplitLyric();

	const static std::wstring _s_GetLyricContent(const std::wstring& _k_SourceLyricLine);
	const static std::wstring _s_GetLyricTime(const std::wstring& _k_SourceLyricLine);

	const static std::wstring _s_ConvertTime(const std::wstring& _k_SourceTime);

	const static std::wstring _s_CombineLyricLine(const std::wstring& _k_ConvertedTime, const std::wstring& _k_LyricLine);
	const static std::wstring _s_CombineAllLyric(const std::vector<std::wstring>& _k_LyricLines, const LinebreakType& _k_LinebreakType = LinebreakType::UNIX);

	const static bool _s_WriteLyric(const std::wstring& _k_LyricAddress, const std::wstring& _k_CombinedLyric);
	const bool _WriteLyric(const std::wstring& _k_CombinedLyric);

public:

	LyricHelper();
	LyricHelper(const std::wstring& k_LyricAddress, const LinebreakType& k_LinebreakType = LinebreakType::UNIX);
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

	const static bool s_ConvertLyric(const std::wstring& k_LyricAddress, const LinebreakType& k_LinebreakType = LinebreakType::UNIX);
	const bool ConvertLyric(const LinebreakType& k_LinebreakType = LinebreakType::UNIX);

};
