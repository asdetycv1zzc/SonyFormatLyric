#pragma once
#include<iostream>
#include<Windows.h>
#include<string>

char* UnicodeToUtf8(const wchar_t* unicode);
bool IsUTF8WithoutBOM(char* _data, int size);