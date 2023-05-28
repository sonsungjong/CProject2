#include "pch.h"
#include <vector>

void main()
{
	std::wstring buf = L"hello world";
	std::wstring& record = buf;

	_tprintf(_T("%s"), record.c_str());
}
