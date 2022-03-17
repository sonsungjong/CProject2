// WinDesk1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WinDeskUI1.h"

// Global Variables:
HINSTANCE hInst;                                // current instance

INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DlgProc);
    return 0;
}


INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_INITDIALOG)
    {

    }
    else if (message == WM_COMMAND)
    {
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
            //TCHAR 제목[128] = _T("제목을 지정하세요");
            //TCHAR 내용[512];

            //GetDlgItemText(hDlg, IDC_EDIT1, 내용, sizeof(내용) / sizeof(내용[0]));
            //MessageBox(hDlg, 내용, 제목, MB_OK);
            DisplayInfo(1, 25);
            DisplayInfo(2, (INT_PTR)_T("이순신"));
        }
    }
    else if (message == WM_CLOSE)
    {
        DestroyWindow(hDlg);
        return TRUE;
    }
    else if (message == WM_DESTROY)
    {
        return TRUE;
    }

    return FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        g_hWnd = hDlg;
        
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void DisplayInfo(int Type, INT_PTR Value)
{
    TCHAR info[128];
    if (Type == 1) {
        _stprintf_s(info, _countof(info), _T("나이는 %lld입니다."), Value);
    }
    else {
        _stprintf_s(info, _countof(info), _T("나이는 %s입니다."), (TCHAR*)Value);
    }
    MessageBox(g_hWnd, info, _T("정보"), MB_OK);
}