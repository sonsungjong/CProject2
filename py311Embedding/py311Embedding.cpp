// py311Embedding.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <Python.h>

#ifdef _DEBUG
#pragma comment(lib, "python311_d.lib")
#else
#pragma comment(lib, "python311.lib")
#endif

std::string UTF8ToMultiByte(const char* utf8Str) {
	// UTF-8 문자열을 멀티바이트로 변환
	int wchars_needed = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, NULL, 0);
	std::wstring wideStr(wchars_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, &wideStr[0], wchars_needed);

	int bytes_needed = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
	std::string multiByteStr(bytes_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &multiByteStr[0], bytes_needed, NULL, NULL);

	return multiByteStr;
}

// 매개변수와 리턴 없는 def함수
void pyNoReturnNoArgs() {
	PyObject* pName, * pModule, * pFunc, * pValue;
	Py_Initialize();

	// sys 모듈을 가져와서 sys.path에 절대경로 추가
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('C:/test')");			// 파이썬 위치 경로 추가

	pName = PyUnicode_FromString("pp");				// pp.py
	pModule = PyImport_Import(pName);
	pFunc = PyObject_GetAttrString(pModule, "test1");				// test1 함수
	pValue = PyObject_CallObject(pFunc, NULL);						// 매개변수를 전달하려 함수 실행
	Py_Finalize();
}

// 매개변수 문자열1
void pyNoReturnStrArgs(const std::string& _strArgs) {
	Py_Initialize();

	PyObject* pName, * pModule, * pFunc, * pValue, * pArgs;
	// sys 모듈을 가져와서 sys.path에 절대경로 추가
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('C:/test')");			// 파이썬 위치 경로 추가

	pName = PyUnicode_FromString("ppa");				// ppa.py
	pModule = PyImport_Import(pName);
	pFunc = PyObject_GetAttrString(pModule, "test2");				// test2 함수
	pArgs = PyTuple_New(1);				// 매개변수 1개
	int args_idx = 0;
	PyTuple_SetItem(pArgs, args_idx, PyUnicode_DecodeLocale(_strArgs.c_str(), NULL));				// EUC-KR 처리
	args_idx++;
	pValue = PyObject_CallObject(pFunc, pArgs);						// 매개변수를 전달하려 함수 실행
	Py_Finalize();
}

// 매개변수 문자열1, 리턴 문자열UTF-8
std::string pyStrReturnStrArgs(const std::string& _strArgs) {
	std::string func_result = "";
	Py_Initialize();

	PyObject* pName, * pModule, * pFunc, * pValue, * pArgs;
	// sys 모듈을 가져와서 sys.path에 절대경로 추가
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('C:/test')");			// 파이썬 위치 경로 추가

	pName = PyUnicode_FromString("ppr");				// ppr.py
	pModule = PyImport_Import(pName);
	pFunc = PyObject_GetAttrString(pModule, "test3");				// test3 함수
	pArgs = PyTuple_New(1);				// 매개변수 1개
	int args_idx = 0;

	PyTuple_SetItem(pArgs, args_idx, PyUnicode_DecodeLocale(_strArgs.c_str(), NULL));				// EUC-KR 처리
	args_idx++;
	pValue = PyObject_CallObject(pFunc, pArgs);						// 매개변수를 전달하려 함수 실행
	const char* pyResult = PyUnicode_AsUTF8(pValue);  // UTF-8로 변환
	Py_Finalize();

	func_result = UTF8ToMultiByte(pyResult);

	return func_result;
}

int main() {
	_putenv("PYTHONHOME=C:/Python311");
	_putenv("PYTHONPATH=C:/Python311/include");

	std::string strj = "{\"1\":\"일\", \"2\":\"이\"}";

	pyNoReturnNoArgs();
	pyNoReturnStrArgs(strj);
	std::string value = pyStrReturnStrArgs(strj);
	printf("%s\n", value.c_str());

	return 0;
}