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

    pyName = PyUnicode_FromString("_00_Sum");           // _00_Sum.py�� PyObject�� ����
    pyModule = PyImport_Import(pyName);             // import
    pyFunc = PyObject_GetAttrString(pyModule, "Sum");           // ������ �Լ��� Sum�� PyObject�� ����
    pyArg = Py_BuildValue("(i, i)", 3, 4);              // int�� ���� 2�� ���� (z : const char*)
    //pyArg = Py_BuildValue("(z)", (const char*)"hello python");
    //pyArg = PyTuple_Pack(2, PyFloat_FromDouble(3), PyFloat_FromDouble(4));               // ����, ����1, ����2
    pyValue = PyObject_CallObject(pyFunc, pyArg);                // pyFunc�� �Ű����� ����
    result = _PyLong_AsInt(pyValue);
    // ���� �׽�Ʈ ��
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