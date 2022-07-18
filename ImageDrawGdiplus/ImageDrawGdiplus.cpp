// ImageDrawGdiplus.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "ImageDrawGdiplus.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;
// #define WIN32_LEAN_AND_MEAN 제거하기

HINSTANCE g_hInst;
RECT gp_rect;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    if (msg == WM_PAINT) {
        hdc = BeginPaint(hWnd, &ps);
        Gdiplus::Bitmap bitmap(gp_rect.right, gp_rect.bottom);      // 더블 버퍼링을 위한 비트맵
        Gdiplus::Graphics graphics(hdc);                // 최종
        Gdiplus::Graphics memDC(&bitmap);               // 더블 버퍼링용

        Gdiplus::SolidBrush whiteBrush(Gdiplus::Color(255, 255, 255, 255));
        memDC.FillRectangle(&whiteBrush, 0, 0, gp_rect.right, gp_rect.bottom);

        Gdiplus::Image img_path = _T("../img/Lenna.png");
        memDC.DrawImage(&img_path, 0, 0, gp_rect.right, gp_rect.bottom);

        Gdiplus::Pen pen(Gdiplus::Color(255, 0, 255, 0), 5.0f);
        memDC.DrawLine(&pen, Gdiplus::Point(0, 100), Gdiplus::Point(500, 100));

        graphics.DrawImage(&bitmap, 0, 0);              // 저장된 것을 한번에 그리기
        
        EndPaint(hWnd, &ps);
        return 0;
    }
    else if (msg == WM_CREATE) {
        RECT r;
        GetClientRect(hWnd, &r);
        gp_rect.right = (float)r.right;
        gp_rect.bottom = (float)r.bottom;

        return 0;
    }
    else if (msg == WM_DESTROY) {
        PostQuitMessage(0);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // Gdi+ 초기화
    ULONG_PTR gpToken;
    GdiplusStartupInput gpsi;
    if (GdiplusStartup(&gpToken, &gpsi, NULL) != Ok) {
        return 0;
    }
    
    g_hInst = hInstance;
    WNDCLASS wc;

    LPCTSTR className = _T("ImgGdiplus");
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));

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

    HWND hWnd = CreateWindow(className, _T("ImgGdiplus"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
    // TODO : Initialize

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(hBrush);
    GdiplusShutdown(gpToken);
    return (int)msg.wParam;
}
