#include "framework.h"
#include "WinApi41MultiThread.h"

HWND hWndMain;

DWORD WINAPI ThreadFunc(void* temp)
{
    HDC hdc;
    BYTE Blue = 0;
    HBRUSH hBrush, hOldBrush;
    hdc = GetDC(hWndMain);
    for (;;) {
        Blue += 5;
        Sleep(20);
        hBrush = CreateSolidBrush(RGB(0, 0, Blue));
        hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        Rectangle(hdc, 10, 10, 400, 200);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBrush);
    }
    ReleaseDC(hWndMain, hdc);
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    DWORD ThreadID;
    HANDLE hThread;
    HDC hdc;
    PAINTSTRUCT ps;

    if (msg == WM_PAINT) {
        hdc = BeginPaint(hWnd, &ps);
        // TODO:

        EndPaint(hWnd, &ps);
        return 0;
    }
    else if (msg == WM_CREATE) {
        hWndMain = hWnd;
        hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);                    // 하나의 프로세스에 추가 쓰레드 생성
        if(hThread != NULL){
            CloseHandle(hThread);
        }
        return TRUE;
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

    HWND hWnd = CreateWindow(className, _T("My Title"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
    // TODO : Initialize

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
