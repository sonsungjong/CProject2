// TTS2.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "TTS2.h"

#pragma warning(disable:4996)
#include <sapi.h>
#include <sphelper.h>

//#include <cstdio>
//#include <string>
//#include <future>
//#include <thread>

HWND* g_hWnd;
void* p_ptr_tts;

void tts1()
{
    (*(CComPtr<ISpVoice>*)p_ptr_tts)->Pause();
    (*(CComPtr<ISpVoice>*)p_ptr_tts)->Resume();
    HRESULT hr = (*(CComPtr<ISpVoice>*)p_ptr_tts)->Speak(_T("첫번째, 첫번째, 첫번째"), SPF_ASYNC | SPF_IS_NOT_XML | SPF_PURGEBEFORESPEAK, 0);
    if (FAILED(hr)) {
        MessageBox(*g_hWnd, _T("TTS오류"), _T("TTS오류"), MB_ICONSTOP);
    }
}

void tts2()
{
    (*(CComPtr<ISpVoice>*)p_ptr_tts)->Pause();
    (*(CComPtr<ISpVoice>*)p_ptr_tts)->Resume();
    HRESULT hr = (*(CComPtr<ISpVoice>*)p_ptr_tts)->Speak(_T("두번째, 두번째, 두번째"), SPF_ASYNC | SPF_IS_NOT_XML | SPF_PURGEBEFORESPEAK, 0);
    if (FAILED(hr)) {
        MessageBox(*g_hWnd, _T("TTS오류"), _T("TTS오류"), MB_ICONSTOP);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static char flag = 0;

    if (msg == WM_PAINT) {
        hdc = BeginPaint(hWnd, &ps);
        // TODO:

        EndPaint(hWnd, &ps);
        return 0;
    }
    else if (msg == WM_LBUTTONDOWN) {
        if (flag == 0) {
            flag = 1;
            tts1();
            //std::thread t1(tts1);
            //t1.join();
            //auto asy = std::async(std::launch::async, tts1);
            //asy.get();
        }
        else if (flag == 1) {
            flag = 0;
            tts2();
            //std::thread t2(tts2);
            //t2.join();
            //auto def = std::async(std::launch::async, tts2);
            //def.get();
        }
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
    g_hWnd = &hWnd;
    // TODO : Initialize
    CoInitialize(NULL);
    CComPtr<ISpVoice> tts_audio;

    p_ptr_tts = &tts_audio;
    HRESULT hr = tts_audio.CoCreateInstance(CLSID_SpVoice);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    tts_audio.Release();
    CoUninitialize();
    DeleteObject(hBrush);
    return (int)msg.wParam;
}
