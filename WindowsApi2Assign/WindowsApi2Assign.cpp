// WindowsApi2Assign.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsApi2Assign.h"

// Global Variables:
HINSTANCE hInst;                                // current instance
MyClass* mc1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    
    switch (umsg)
    {
    case WM_SIZE:
        GetClientRect(hWnd, &(mc1->rect));              // 현재 화면크기 얻기
        InvalidateRect(hWnd, NULL, TRUE);
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
            return DefWindowProc(hWnd, umsg, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        SetTextAlign(hdc, TA_CENTER);
        TextOut(hdc, mc1->rect.right / 2, mc1->rect.bottom / 2, TEXT("Center String"), 13);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        delete mc1;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, umsg, wParam, lParam);
    }
    return 0;
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    WNDCLASS wc;

    LPCTSTR className = _T("assigntextout");
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 155, 0));

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

    HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, className, _T("My Title"), WS_OVERLAPPEDWINDOW, 0, 0, 300, 300, nullptr, (HMENU)nullptr, hInstance, nullptr);

    mc1 = new MyClass();
    GetClientRect(hWnd, &(mc1->rect));

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
