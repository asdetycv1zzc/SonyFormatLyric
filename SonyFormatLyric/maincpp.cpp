#include<iostream>
#include<time.h>
#include<string>
#include<Windows.h>
#include"LyricHelper.h"
#include"DealMiscellousProblems.h"
using namespace std;

int wmain(int argc, wchar_t* argv[])
{
	double beg, end;
	size_t success = 0, fail = 0;
	beg = clock();
	if (argc <= 1)
	{

		wchar_t _temp[30000] = { 0 };
		wcin.imbue(locale("chs"));
		wcout << L"Using Input Mode: Maunal\nPlease input complete lyric addresses line by line (Press Ctrl + Z to stop): \n\n";
		while (wcin.getline(_temp, 30000))
		{
			wstring _wideAddress(_temp);
			auto _cl = LyricHelper(_wideAddress);
			_cl.ConvertLyric();
			auto _err = _cl.GetConvertionStatus();
			if (!_err)
			{
				wprintf(L"%s Failed: ", _cl.GetLyricShortAddress().c_str());
				switch (_err)
				{
				case ConvertionStatus::BackupFailed:
				{
					wprintf(L"BackupFailed\n");
					break;
				}
				case ConvertionStatus::OpenFileFailed:
				{
					wprintf(L"OpenFileFailed\n");
					break;
				}
				case ConvertionStatus::FileNotFound:
				{
					wprintf(L"BackupFailed\n");
					break;
				}
				case ConvertionStatus::NoContent:
				{
					wprintf(L"NoContent\n");
					break;
				}
				case ConvertionStatus::NotLyricFormat:
				{
					wprintf(L"NotLyricFormat\n");
					break;
				}
				case ConvertionStatus::UndefinedError:
				{
					wprintf(L"UndefinedError\n");
					break;
				}
				default:
				{
					wprintf(L"Maybe you are suffering from a memory leak or high CPU usage? The programme did not even recognize this problem.\n");
					break;
				}
				}
				
				fail += 1;
				continue;
			}
			else
			{
				success += 1;
			}
		}
	}

	for (int i = 1; i < argc; i++)
	{
		if (i == 1)wcout << L"Using Input Mode: Auto \nLyrics Count: " << argc - 1 << L"\n";
		wstring _wideAddress(argv[i]);
		wcout.imbue(locale("chs"));
		
		auto _cl = LyricHelper(_wideAddress);
		_cl.ConvertLyric();
		auto _err = _cl.GetConvertionStatus();
		if (!_err)
		{
			wprintf(L"%s Failed: ", _cl.GetLyricShortAddress().c_str());
			switch (_err)
			{
			case ConvertionStatus::BackupFailed:
			{
				wprintf(L"BackupFailed\n");
				break;
			}
			case ConvertionStatus::OpenFileFailed:
			{
				wprintf(L"OpenFileFailed\n");
				break;
			}
			case ConvertionStatus::FileNotFound:
			{
				wprintf(L"BackupFailed\n");
				break;
			}
			case ConvertionStatus::NoContent:
			{
				wprintf(L"NoContent\n");
				break;
			}
			case ConvertionStatus::NotLyricFormat:
			{
				wprintf(L"NotLyricFormat\n");
				break;
			}
			case ConvertionStatus::UndefinedError:
			{
				wprintf(L"UndefinedError\n");
				break;
			}
			default:
			{
				wprintf(L"Maybe you are suffering from a memory leak or high CPU usage? The programme did not even recognize this problem.\n");
				break;
			}
			}

			fail += 1;
			continue;
		}
		else
		{
			success += 1;
		}
	}
	end = clock();
	wcout << L"Time Used: " << (end - beg) << L"\n";
	wcout << success << L" Success, " << fail << L" Fail\n";
	Sleep(3000);
	return 0;
}