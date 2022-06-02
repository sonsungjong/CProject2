// Gdiplus1.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Gdiplus1.h"
#include <gdiplus.h>                            // GDI+ �������
#pragma comment(lib, "gdiplus")         // GDI+ ���̺귯�� ����
using namespace Gdiplus;                    // GDI+ ���ӽ����̽�
// #define WIN32_LEAN_AND_MEAN�� �����ϱ�

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    if (msg == WM_PAINT) {
        hdc = BeginPaint(hWnd, &ps);
        // GDI+
        Graphics* pGraphic = new Graphics(hdc);

        // A, R, G, B
        Pen* pPen = new Pen(Color(255, 255, 0, 0), 3);              // ������, ���� 3

        pGraphic->SetSmoothingMode(SmoothingModeAntiAlias);
        pGraphic->DrawLine(pPen, 50, 50, 200, 107);

        delete pGraphic;
        delete pPen;

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
    // Gdi+ �ʱ�ȭ�۾�
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

    HWND hWnd = CreateWindow(className, _T("My Title"), WS_OVERLAPPEDWINDOW, 50, 50, 500, 230, NULL, (HMENU)NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(hBrush);
    // Gdi+ �����۾�
    GdiplusShutdown(gpToken);
    return (int)msg.wParam;
}