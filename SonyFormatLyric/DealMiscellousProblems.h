#pragma once
#include<iostream>
#include<string>
#include<Windows.h>

std::wstring stringToWstring(const std::string& str);
bool IsUTF8WithoutBOM(char* _data, int size);
