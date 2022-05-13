// WindowsApi2background.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsApi2background.h"

HINSTANCE g_hInst;

// 백그라운드 작업을 수행할 콜백함수 (타이머)
void CALLBACK TimerProc1(HWND hWnd, UINT msg, UINT idEvent, DWORD dwTime)
{
    HDC hdc;
    int i;
    hdc = GetDC(hWnd);
    for (i = 0; i < 1000; i++) {
        SetPixel(hdc, rand() % 500, rand() % 400, RGB(rand() % 256, rand() % 256, rand() % 256));
    }
    ReleaseDC(hWnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static TCHAR str[128], sansu[64];
    static int num1, num2;
    
    switch (msg)
    {
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
        TextOut(hdc, 10, 10, str, _tcslen(str));
        EndPaint(hWnd, &ps);
    }
    return 0;
    case WM_MBUTTONDOWN:
        SetTimer(hWnd, 3, 3000, NULL);
        return 0;
    case WM_RBUTTONDOWN:
        _tcscpy_s(str, 11, _T("왼쪽 버튼을 눌렀음"));
        InvalidateRect(hWnd, NULL, TRUE);
        SetTimer(hWnd, 13, 3000, NULL);
        return 0;
    case WM_TIMER:
        if (wParam == 13) {
            KillTimer(hWnd, 13);
            _tcscpy_s(str, 1, _T(""));
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == 3) {
            SetTimer(hWnd, 6, 3000, NULL);
            hdc = GetDC(hWnd);
            num1 = rand() % 30;
            num2 = rand() % 30;
            int num3 = num1 + num2;
            _stprintf_s(sansu, 64 , _T("%d + %d = %d"), num1, num2, num3);
            TextOut(hdc, 50, 50, sansu, _tcslen(sansu));
            ReleaseDC(hWnd, hdc);
        }
        else if (wParam == 6) {
            KillTimer(hWnd, 6);
            hdc = GetDC(hWnd);
            TCHAR sum[128];
            int num3 = 0, num4 = 0, num5 = 0;
            num3 = rand() % 10;
            num4 = rand() % 10;
            num5 = num3 + num4;
            _stprintf_s(sum, 128, _T("추가 : %d + %d = %d"), num3, num4, num5);
            TextOut(hdc, 50, 100, sum, _tcslen(sansu));
            ReleaseDC(hWnd, hdc);
        }
        return 0;
    case WM_LBUTTONDOWN:
        // 클릭시 원 생성
        hdc = GetDC(hWnd);
        Ellipse(hdc, LOWORD(lParam) - 10, HIWORD(lParam) - 10, LOWORD(lParam) + 10, HIWORD(lParam) + 10);
        ReleaseDC(hWnd, hdc);
        return 0;
    case WM_DESTROY:
        KillTimer(hWnd, 1);             // 콜백함수 1번
        KillTimer(hWnd, 3);             // 타이머 3번
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
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    g_hInst = hInstance;
    HWND hWnd;
    MSG msg;
    WNDCLASS wc;
    LPCTSTR className = _T("__ClassName_");

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

    hWnd = CreateWindow(className, className, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    SetTimer(hWnd, 1, 100, (TIMERPROC)TimerProc1);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
