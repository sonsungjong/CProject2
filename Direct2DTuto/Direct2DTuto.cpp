#include "framework.h"
#include "Direct2DTuto.h"
#include "Graphics.h"

// Tuto 6 : Real Time Game Loop

Graphics* graphics;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    if (msg == WM_PAINT) {
        //hdc = BeginPaint(hWnd, &ps);
        // TODO:
        graphics->BeginDraw();

        graphics->ClearScreen(0.0f, 0.0f, 0.5f);

        for (UINT i = 0; i < 1000; ++i)
        {
            graphics->DrawCircle(rand()%800, rand() % 600, rand() % 100, 
                (rand() % 100)/ 100.0f, 
                (rand() % 100) / 100.0f,
                (rand() % 100) / 100.0f,
                1.0f);
        }


        graphics->EndDraw();
        //EndPaint(hWnd, &ps);
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
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    LPCTSTR className = _T("MainWindow");
    //HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = className;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClassEx(&wc);

    RECT rect = {0, 0, 800, 600};
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

    HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, className, _T("DirectX Tute!"), WS_OVERLAPPEDWINDOW, 
        100, 100, rect.right - rect.left, rect.bottom - rect.top, NULL, (HMENU)NULL, hInstance, NULL);
    // TODO : Initialize

    if (!hWnd) {
        return -1;
    }

    graphics = new Graphics;
    if (!graphics->Init(hWnd))
    {
        delete graphics;
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);
    //UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        //TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete graphics;

    return (int)msg.wParam;
}


