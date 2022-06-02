// Gdiplus2.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Gdiplus2.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;
// #define WIN32_LEAN_AND_MEAN를 제거하기

void GdiplusDrawLine(HDC hdc)
{
    Graphics graphics(hdc);
    Pen pen(Color(255, 0, 0, 255));
    graphics.DrawLine(&pen, 0, 0, 200, 100);
}

void GdiplusDrawRectangle(HDC hdc) 
{
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    Pen pen(Color(255, 0, 0, 0), 10.0f);           // 두께 10의 검은펜
    graphics.DrawRectangle(&pen, 30, 30, 100, 100);

    pen.SetLineJoin(LineJoinRound);
    graphics.DrawRectangle(&pen, 170, 30, 100, 100);
}

void GdiplusDrawEllipse(HDC hdc)
{
    Graphics graphics(hdc);
    Pen pen(Color(255, 0, 0, 0), 10.0f);
    graphics.DrawEllipse(&pen, 30, 230, 100, 100);
    graphics.DrawEllipse(&pen, 150, 230, 200, 100);
}

void GdiplusDrawPie(HDC hdc)
{
    Graphics graphics(hdc);
    Pen blackPen(Color(255, 0, 0, 0), 2.0f);
    Pen grayPen(Color(255, 192, 192, 192), 15.0f);
    graphics.DrawEllipse(&grayPen, 30, 400, 150, 150);

    // 원호
    graphics.DrawArc(&blackPen, 30, 400, 150, 150, 0.0f, 90.0f);
    // 부채꼴
    graphics.DrawPie(&blackPen, 30, 400, 150, 150, 180.0f, 90.0f);
}

void GdiplusDrawPolygon(HDC hdc)
{
    Graphics graphics(hdc);
    Pen pen(Color(255, 0, 0, 0), 5.0f);
    Point points[6] = {
        Point(430, 30),
        Point(580, 30),
        Point(580, 130),
        Point(530, 130),
        Point(530, 80),
        Point(430, 80),
    };
    graphics.DrawPolygon(&pen, points, 6);
}

void GdiplusDrawString(HDC hdc)
{
    Graphics graphics(hdc);
    SolidBrush brush(Color(255, 255, 0, 255));              // 분홍색
    FontFamily fontFamily(_T("Times New Roman"));
    Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
    PointF pointF(10.0f, 10.0f);

    graphics.DrawString(_T("Hello Gdiplus"), -1, &font, pointF, &brush);
}

void GdiplusDrawImage(HDC hdc)
{
    Graphics graphics(hdc);
    Image image(_T("../photo.jpg"));

    //graphics.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
    graphics.DrawImage(&image, 330, 330, 150, 150);
}

void GdiplusDrawImage2(HDC hdc)
{
    Graphics graphics(hdc);
    Image image(_T("../photo.jpg"));
    TextureBrush tBrush(&image);
    Pen texturedPen(&tBrush, 10);

    //graphics.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
    //graphics.DrawImage(&image, 330, 330, 150, 150);
    graphics.DrawEllipse(&texturedPen, 450, 230, 100, 100);
    graphics.FillEllipse(&tBrush, 550, 230, 100, 100);
}

void GdiplusFillRectangle(HDC hdc)
{
    Graphics graphics(hdc);
    SolidBrush solidBrush(Color(255, 255, 0, 0));
    graphics.FillEllipse(&solidBrush, 10, 150, 100, 60);
}

// https://docs.microsoft.com/ko-kr/windows/win32/gdiplus/-gdiplus-drawing-a-line-filled-with-a-texture-use

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    if (msg == WM_PAINT) {
        hdc = BeginPaint(hWnd, &ps);
        // GDI+
        GdiplusDrawLine(hdc);
        GdiplusDrawRectangle(hdc);
        GdiplusDrawEllipse(hdc);
        GdiplusDrawPie(hdc);
        GdiplusDrawPolygon(hdc);
        GdiplusDrawString(hdc);
        GdiplusDrawImage(hdc);
        GdiplusDrawImage2(hdc);
        GdiplusFillRectangle(hdc);

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