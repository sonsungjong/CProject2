// WinDeskUI3.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WinDeskUI3.h"

HINSTANCE g_hInst;
WinDeskUI3 wdu3;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static int idx = 0;
    static int x = 0, y = 0;

    if (msg == WM_PAINT) {
        hdc = BeginPaint(hWnd, &ps);
        //MoveWindow(hWnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), TRUE);
        // TODO:
        idx = GetSystemMetrics(SM_NETWORK);
        wdu3.m_screen_size_x = GetSystemMetrics(SM_CXSCREEN);
        wdu3.m_screen_size_y = GetSystemMetrics(SM_CYSCREEN);
        _stprintf_s(wdu3.m_msg_str, 256 ,_T("현재 X축은 %d, Y축은 %d, 화면크기 (%d, %d)"), wdu3.m_screen_size_x, wdu3.m_screen_size_y, x, y);
        TextOut(hdc, 100, 100, wdu3.m_msg_str, _tcslen(wdu3.m_msg_str));
        EndPaint(hWnd, &ps);
        return 0;
    }
    else if (msg == WM_TIMER) {
        if (wParam == 1) {
            InvalidateRect(hWnd, NULL, FALSE);
        }
    }
    else if (msg == WM_LBUTTONDOWN) {
        x = GetSystemMetrics(SM_CXSCREEN);
        y = GetSystemMetrics(SM_CYSCREEN);
        MoveWindow(hWnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), TRUE);
    }
    else if (msg == WM_RBUTTONDOWN) {
        x = GetSystemMetrics(SM_CXSCREEN)/2;
        y = GetSystemMetrics(SM_CYSCREEN)/2;
        MoveWindow(hWnd, 0, 0, GetSystemMetrics(SM_CXSCREEN)/2, GetSystemMetrics(SM_CYSCREEN)/2, TRUE);
    }
    else if (msg == WM_CREATE) {
        wdu3.m_screen_size_x = GetSystemMetrics(SM_CXSCREEN);
        wdu3.m_screen_size_y = GetSystemMetrics(SM_CYSCREEN);
        _stprintf_s(wdu3.m_msg_str, 256, _T("현재 X축은 %d, Y축은 %d"), wdu3.m_screen_size_x, wdu3.m_screen_size_y);
        MessageBox(hWnd, wdu3.m_msg_str, _T("현재 해상도 가져오기"), NULL);
        SetTimer(hWnd, 1, 1000, NULL);
    }
    else if (msg == WM_COMMAND) {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        return 0;
    }
    else if (msg == WM_DESTROY) {
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    WNDCLASS wc;

    g_hInst = hInstance;
    LPCTSTR className = _T("winmain_format");
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = hBrush;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = className;
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, className, _T("My Title"), WS_POPUP, 0, 0, 500, 500, nullptr, (HMENU)nullptr, hInstance, nullptr);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(hBrush);
    return (int)msg.wParam;
}

