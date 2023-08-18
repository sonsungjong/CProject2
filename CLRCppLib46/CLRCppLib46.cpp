#include "pch.h"

using namespace CsClassLib;

#include <msclr/marshal_cppstd.h>
#include "CLRCppLib46.h"

std::wstring MyClrClass::Func1(std::wstring str)
{
	MyDll^ dll = gcnew MyDll();
	System::String^ sys_str = gcnew System::String(str.c_str());

	System::String^ result = dll->Func1(sys_str);
	return msclr::interop::marshal_as<std::wstring>(result);
}

void MyClrClass::Func2()
{
	MyDll^ dll = gcnew MyDll();
	dll->Func2();
}

int MyClrClass::Func3(int a, int b)
{
	MyDll^ dll = gcnew MyDll();
	return dll->Func3(a, b);
}
