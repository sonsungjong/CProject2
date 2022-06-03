// TTS4.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"

#include "TTS4.h"
#define ID_EDIT1 44004
#define ID_BUTTON1 44005
#define ID_BUTTON2 44006
HINSTANCE g_hInst;
HWND hEdit;
TTSClass* ttsc1;
TCHAR str[512];

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    if (msg == WM_PAINT) {
        hdc = BeginPaint(hWnd, &ps);
        // TODO:

        EndPaint(hWnd, &ps);
        return 0;
    }
    else if (msg == WM_LBUTTONDOWN) {
        SetFocus(hEdit);
    }
    else if (msg == WM_COMMAND) {
        switch (LOWORD(wParam))
        {
        case ID_BUTTON1:
            GetWindowText(hEdit, str, 512);
            ttsc1->StartTTS(str);
            //MessageBox(hWnd, str, str, MB_OK);
            break;
        case ID_BUTTON2:
            SetWindowText(hEdit, _T(""));
            ttsc1->StartTTS((TCHAR*)_T("지우기가 완료되었습니다."));
            //MessageBox(hWnd, _T("지우기 완료"), _T("버튼2"), MB_OK);
            break;
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
    g_hInst = hInstance;

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

    HWND hWnd = CreateWindow(className, _T("TTS 테스트"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
    hEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 10, 10, 300, 25, hWnd, (HMENU)ID_EDIT1, g_hInst, NULL);
    CreateWindow(TEXT("button"), _T("TTS 실행"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 315, 10, 100, 25, hWnd, (HMENU)ID_BUTTON1, g_hInst, NULL);
    CreateWindow(TEXT("button"), _T("지우기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 420, 10, 100, 25, hWnd, (HMENU)ID_BUTTON2, g_hInst, NULL);

    // TODO : Initialize
    ttsc1 = new TTSClass();

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // TTS함수 사용법
    ttsc1->StartTTS((TCHAR*)_T("윈도우 시작완료"));

    SetFocus(hEdit);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete ttsc1;
    DeleteObject(hBrush);
    return (int)msg.wParam;
}
