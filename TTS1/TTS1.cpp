// TTS1.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "TTS1.h"

#pragma warning(disable:4996)
#include <sapi.h>
#include <sphelper.h>


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
        CoInitialize(NULL);
        CComPtr<ISpVoice> ptr_tts;
        HRESULT hr = ptr_tts.CoCreateInstance(CLSID_SpVoice);
        if (SUCCEEDED(hr)) {
            ptr_tts->Speak(_T("안녕하세요? 저는 TTS 입니다."), SPF_DEFAULT, 0);
            ptr_tts.Release();
        }
        CoUninitialize();
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

    HWND hWnd = CreateWindow(className, _T("TTS 테스트"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
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
