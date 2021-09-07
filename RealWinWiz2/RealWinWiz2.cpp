// RealWinWiz2.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include <shellapi.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY) {   // WM_CLOSE 처리후에 들어오는 메시지
        PostQuitMessage(0);  // 자신의 메시지 큐에 WM_QUIT 메시지를 넣는다!
    }
    else if (uMsg == WM_CREATE) {
        HWND h_wnd_fw = FindWindow(L"Notepad", NULL);
        if (h_wnd_fw == NULL) {
            ShellExecute(NULL, L"open", L"Notepad", 0, 0, SW_SHOW);
        }
        else {
            PostMessage(h_wnd_fw, WM_CLOSE, 0, 0);
            ShellExecute(NULL, L"open", L"Notepad", 0, 0, SW_SHOW);
        }
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        HWND h_wnd_fw = FindWindow(L"Notepad", NULL);
        HWND h_wnd_fwe = FindWindowEx(h_wnd_fw, NULL, L"Edit", NULL);
        if (h_wnd_fw != 0) {
            HDC h_dc = GetDC(hWnd);
            SetWindowText(h_wnd_fw, L"노트패드 제목 바꾸기");
            SendMessage(h_wnd_fwe, WM_SETTEXT, 0, (LPARAM)L"글방해하기~~");
            Rectangle(h_dc, 10, 10, 100, 100);
            ReleaseDC(hWnd, h_dc);
        }
    }
    else if (uMsg == WM_MBUTTONDOWN) {
        HWND h_wnd_fw = FindWindow(L"NotePad", NULL);
        if (h_wnd_fw != 0) {
            SetWindowPos(h_wnd_fw, HWND_TOPMOST, 0, 0, 300, 300, NULL);
        }
    }
    else if (uMsg == WM_RBUTTONDOWN) {
        HWND h_wnd_fw = FindWindow(L"Notepad", NULL);
        if (h_wnd_fw != 0) {
            PostMessage(h_wnd_fw, WM_CLOSE, 0, 0);
        }
    }

    // 개발자가 처리하지 않은 메시지들을 처리한다!
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 윈도우 클래스 등록 작업!!
    WNDCLASS wc;

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;     // 배경색 지정
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);                // 커서 지정
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // 응용 프로그램 로고 아이콘 지정
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;    // 기본 메시지 처리기 함수 지정
    wc.lpszClassName = L"노트패드조작";    // 윈도우 클래스 이름 지정
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);    // 윈도우 클래스 등록

    // 윈도우 생성!
    HWND hWnd = CreateWindowW(L"노트패드조작", L"노트패드 조져 부리기~",
        WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);   // 윈도우를 화면에 어떻게 출력할 것인가 지정!
    UpdateWindow(hWnd);    // WM_PAINT 메시지가 대기 상태에 있다면 즉시 수행하도록 한다!

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {   // WM_QUIT 메시지가 발생할때까지 반복한다.
        TranslateMessage(&msg);   // 키보드 메시지 발생시에 추가 메시지를 발생할지 여부 체크
        DispatchMessage(&msg);   // 발생된 메시지를 처리한다!
    }
    return msg.wParam;
}