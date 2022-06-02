// Gdiplus3.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Gdiplus3.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;
// #define WIN32_LEAN_AND_MEAN를 제거하기

void DrawFullImage(HWND hWnd, HDC hdc, Image imagePath)
{
    RECT rect;
    //HDC mydc = GetDC(hWnd);
    Graphics graphics(hdc);
    GetClientRect(hWnd, &rect);

    //graphics.DrawImage(&imagePath, 0, 0, imagePath.GetWidth(), imagePath.GetHeight());
    graphics.DrawImage(&imagePath, 0, 0, rect.right, rect.bottom);
}

void DrawImage(HWND hWnd, Image imagePath, int x, int y, int width, int height)
{
    RECT rect;
    HDC mydc = GetDC(hWnd);
    Graphics graphics(mydc);
    GetClientRect(hWnd, &rect);

    graphics.DrawImage(&imagePath, x, y, width, height);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    if (msg == WM_PAINT) {
        hdc = BeginPaint(hWnd, &ps);
        // GDI+
        DrawFullImage(hWnd, hdc, _T("../img/lenna.png"));
        DrawImage(hWnd, _T("../img/photo.jpg"), 10, 10, 500, 500);

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

    WNDCLASS wc;

    LPCTSTR className = _T("winmain_format");
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

    HWND hWnd = CreateWindow(className, _T("My Title"), WS_OVERLAPPEDWINDOW, 50, 50, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(hBrush);
    // Gdi+ 정리작업
    GdiplusShutdown(gpToken);
    return (int)msg.wParam;
}