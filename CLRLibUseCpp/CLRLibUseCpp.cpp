#include <iostream>
#include "CLRCppLib46.h"

#pragma comment(lib, "CLRCppLib46.lib")

// x64 �ȿ� Release ��� �ȿ� C#�� dll�� �����ؾ���

int main()
{
	MyClrClass m;

	std::wstring str = m.Func1(L"hello world");


	int result = m.Func3(1, 2);
	wprintf(L"%d, %s\n", result, str.c_str());

	m.Func2();

	return 0;
}