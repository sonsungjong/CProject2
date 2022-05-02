// WindowsApi2keydown.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsApi2keydown.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static int x = 100;
    static int y = 100;
    static TCHAR* text = (TCHAR*)TEXT("A");

    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_LEFT) {
            x -= 8;
        }
        else if (wParam == VK_RIGHT) {
            x += 8;
        }
        else if (wParam == VK_UP) {
            y -= 8;
        }
        else if (wParam == VK_DOWN) {
            y += 8;
        }
        else if (wParam == VK_SPACE) {
            text == (TCHAR*)_T("A") ? text = (TCHAR*)_T("#") : text = (TCHAR*)_T("A");
        }
        InvalidateRect(hWnd, NULL, TRUE);                   // FALSE로 하면 이전 화면이 지워지지 않음
        return 0;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }
    return 0;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        TextOut(hdc, x, y, text, 1);
        EndPaint(hWnd, &ps);
    }
    return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    HWND hWnd;
    MSG msg;
    WNDCLASS wc;
    LPCTSTR className = _T("DOWN");

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = className;
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    hWnd = CreateWindow(className, _T("키보드/마우스 입력"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}