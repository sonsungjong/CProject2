// CallPython.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "CallPython.h"
#include <Python.h>
#include <string>
typedef std::basic_string<TCHAR> TSTRING;

// Global Variables:
HINSTANCE hInst;                                // current instance
PyObject *pyName, *pyModule, *pyFunc, *pyArg, *pyValue;
int result;
TCHAR str[16];

INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\"C:\\Users\\user\\AppData\\Local\\Programs\\Python\\Python38\")");
    PyRun_SimpleString("sys.path.append(\"D:\\python\\hundred\")");

    pyName = PyUnicode_FromString("_00_Sum");           // _00_Sum.py를 PyObject로 생성
    pyModule = PyImport_Import(pyName);             // import
    pyFunc = PyObject_GetAttrString(pyModule, "Sum");           // 실행할 함수인 Sum을 PyObject에 전달
    pyArg = Py_BuildValue("(i, i)", 3, 4);              // int형 인자 2개 전달 (z : const char*)
    //pyArg = Py_BuildValue("(z)", (const char*)"hello python");
    //pyArg = PyTuple_Pack(2, PyFloat_FromDouble(3), PyFloat_FromDouble(4));               // 갯수, 인자1, 인자2
    pyValue = PyObject_CallObject(pyFunc, pyArg);                // pyFunc에 매개변수 전달
    result = _PyLong_AsInt(pyValue);
    // 아직 테스트 중
    return 0;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
    
    switch (msg){
    case WM_CLOSE: 
        Py_Finalize();
        DestroyWindow(hwnd);
        break;
    case WM_INITDIALOG: {
        
        _stprintf_s(str, _T("%d"), result);
        SetWindowText(GetDlgItem(hwnd, IDC_PRINT), str);
        
    }
    break;
    }
    return FALSE;
}