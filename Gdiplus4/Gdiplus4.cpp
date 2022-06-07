// Gdiplus4.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Gdiplus4.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;
// #define WIN32_LEAN_AND_MEAN를 제거하기

HINSTANCE g_hInst;
RECT* gp_rect;
HWND hEdit1, hEdit2;

void DrawBkImage(Graphics* graphics, Image imagePath)
{
    graphics->DrawImage(&imagePath, 0, 0, gp_rect->right, gp_rect->bottom);
}

void DrawImage(Graphics* graphics, Image imagePath, int x, int y, int width, int height)
{
    graphics->DrawImage(&imagePath, x, y, width, height);
}

void GdiplusDrawString(Graphics* graphics)
{
    SolidBrush brush(Color(255, 255, 0, 255));              // 분홍색
    FontFamily fontFamily(_T("Times New Roman"));                       // 폰트
    Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);                // 크기
    PointF pointF((gp_rect->right) / 3, gp_rect->bottom / 3 + gp_rect->bottom / 3 / 1.8);                                                        // 위치
    PointF pointF2((gp_rect->right) / 3, gp_rect->bottom / 3 + gp_rect->bottom / 3 / 1.5);                                                        // 위치

    graphics->DrawString(_T("아이디"), -1, &font, pointF, &brush);                // 글자
    graphics->DrawString(_T("비밀번호"), -1, &font, pointF2, &brush);                // 글자
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    if (msg == WM_PAINT) {
        hdc = BeginPaint(hWnd, &ps);
        TCHAR* str = (TCHAR*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        // GDI+
        Graphics grapics(hdc);
        GetClientRect(hWnd, gp_rect);
        
        //_stprintf_s(str, 256, _T("%d"), g_rect.bottom);
        //MessageBox(hWnd, str, str, NULL);
        DrawBkImage(&grapics, _T("../img/sea.jpg"));
        DrawImage(&grapics, _T("../img/photo.jpg"), (gp_rect->right) / 3, (gp_rect->bottom) / 3, (gp_rect->right) / 3, (gp_rect->bottom) / 3);
        GdiplusDrawString(&grapics);
        EndPaint(hWnd, &ps);
        return 0;
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
    // Gdi+ 초기화작업
    ULONG_PTR gpToken;
    GdiplusStartupInput gpsi;
    if (GdiplusStartup(&gpToken, &gpsi, NULL) != Ok) {
        return 0;
    }

    g_hInst = hInstance;
    WNDCLASS wc;

    LPCTSTR className = _T("gdiplus4");

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = className;
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    RECT rect;

    HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, className, _T("My Title"), WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, (HMENU)nullptr, hInstance, nullptr);
    GetClientRect(hWnd, &rect);

    hEdit1 = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rect.right/3 + rect.right/3/5, rect.bottom/3+rect.bottom/3/1.8, rect.right/5, rect.bottom/40, hWnd, (HMENU)IDC_EDIT1, g_hInst, nullptr);
    hEdit2 = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rect.right/3 + rect.right/3/5, rect.bottom/3 + rect.bottom/3/1.5, rect.right/5, rect.bottom/40, hWnd, (HMENU)IDC_EDIT2, g_hInst, nullptr);
    CreateWindow(_T("BUTTON"), _T("버튼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect.right/3+rect.right/3/1.38, rect.bottom / 3 + rect.bottom / 3 / 1.3, 100, 30, hWnd, (HMENU)IDC_BUTTON1, g_hInst, nullptr);

    gp_rect = &rect;
    TCHAR p_str[256];
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)p_str);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Gdi+ 정리작업
    GdiplusShutdown(gpToken);
    return (int)msg.wParam;
}