// TimeAPI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include <ctime>
#include "TimeAPI.h"

#define MI_TIMER_INVENTADO      117

time_t actualTime = time(NULL);
struct tm tmCurTime;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

// Global Variables:
HINSTANCE hInst;                                // current instance
bool isClosed = false;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    hInst = hInstance;

    HWND miVentana = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

    SetTimer(miVentana, MI_TIMER_INVENTADO, 1000, (TIMERPROC)NULL);

    ShowWindow(miVentana, nCmdShow);
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_TIMER: {
        time(&actualTime);
        //timeInfo = localtime(&actualTime);
        localtime_s(&tmCurTime, &actualTime);
        
        TCHAR bufferTime[80];
        _tcsftime(bufferTime, _countof(bufferTime) , _T("%Y-%m-%d %I:%M:%S"), &tmCurTime);
        SetWindowText(GetDlgItem(hwnd, IDC_TIMER), bufferTime);
    }
        break;
    case WM_CLOSE:
        KillTimer(hwnd, MI_TIMER_INVENTADO);
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(117);
        break;
    default:
        DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return FALSE;
}