#pragma once

// extern "C" __declspec(dllexport) void Run(int num = 0);

class MyClrClass
{
public:
	std::wstring Func1(std::wstring str);
	void Func2();
	int Func3(int a, int b);
};

//namespace MyName {
//	public ref class CppClrClass
//	{
//	public:
//		void UseCs();
//	};
//}
