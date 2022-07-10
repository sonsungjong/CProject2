// OMok2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "OMok2.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

#define X_COUNT         19
#define Y_COUNT         19
#define START_X         50
#define START_Y         50
#define INTERVAL        26
#define HALF_INTERVAL       INTERVAL/2
#define XPOS(x) (START_X + (x) * INTERVAL)
#define YPOS(y) (START_Y + (y) * INTERVAL)

unsigned char g_dol[Y_COUNT][X_COUNT];
unsigned char g_step;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    if (msg == WM_PAINT) {
        hdc = BeginPaint(hWnd, &ps);
        // TODO:
        Graphics graphics(hdc);
        Pen pen(Color(0, 0, 0), 1.0f);
        
        for (int x = 0; x < X_COUNT; x++) {
            //MoveToEx(hdc, XPOS(x), YPOS(0), NULL);
            //LineTo(hdc, XPOS(x), YPOS(Y_COUNT - 1));
            graphics.DrawLine(&pen, XPOS(x), YPOS(0), XPOS(x), YPOS(Y_COUNT - 1));
        }

        for (int y = 0; y < Y_COUNT; y++) {
            //MoveToEx(hdc, XPOS(0), YPOS(y), NULL);
            //LineTo(hdc, XPOS(X_COUNT - 1), YPOS(y));
            graphics.DrawLine(&pen, XPOS(0), YPOS(y), XPOS(X_COUNT - 1), YPOS(y));
        }

        for (int y = 0; y < Y_COUNT; y++) {
            for (int x = 0; x < X_COUNT; x++) {
                if (g_dol[y][x] > 0) {
                    if (g_dol[y][x] == 1) {
                        SolidBrush solidBrush(Color(255, 0, 0, 0));
                        graphics.FillEllipse(&solidBrush, XPOS(x) - HALF_INTERVAL, YPOS(y) - HALF_INTERVAL, XPOS(x) + HALF_INTERVAL - (XPOS(x) - HALF_INTERVAL), YPOS(y) + HALF_INTERVAL - (YPOS(y) - HALF_INTERVAL));
                    }
                    else {
                        SolidBrush solidBrush(Color(255, 255, 255, 255));
                        graphics.FillEllipse(&solidBrush, XPOS(x) - HALF_INTERVAL, YPOS(y) - HALF_INTERVAL, XPOS(x) + HALF_INTERVAL - (XPOS(x) - HALF_INTERVAL), YPOS(y) + HALF_INTERVAL - (YPOS(y) - HALF_INTERVAL));
                    }
                }
            }
        }
        EndPaint(hWnd, &ps);
        return 0;
    }
    else if (msg == WM_LBUTTONDOWN) {
        int a_x = LOWORD(lParam);
        int a_y = HIWORD(lParam);
        if (a_x > (XPOS(0) - HALF_INTERVAL) && a_y > (YPOS(0) - HALF_INTERVAL) && a_x < (XPOS(X_COUNT - 1) + HALF_INTERVAL) && a_y < (YPOS(Y_COUNT - 1) + HALF_INTERVAL)) {
            int x = (a_x - START_X + HALF_INTERVAL) / INTERVAL;
            int y = (a_y - START_Y + HALF_INTERVAL) / INTERVAL;
            if (g_dol[y][x] == 0) {
                g_dol[y][x] = g_step + 1;
                g_step = !g_step;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
    }
    else if (msg == WM_DESTROY) {
        PostQuitMessage(0);
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
    // GDI+
    ULONG_PTR gpToken;
    GdiplusStartupInput gpsi;
    if (GdiplusStartup(&gpToken, &gpsi, NULL) != Ok) {
        return 0;
    }

    WNDCLASS wc;

    LPCTSTR className = _T("omok2");
    HBRUSH hBrush = CreateSolidBrush(RGB(244, 176, 77));

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

    HWND hWnd = CreateWindow(className, _T("OMOK"), WS_OVERLAPPEDWINDOW, 50, 50, 600, 600, NULL, (HMENU)NULL, hInstance, NULL);
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