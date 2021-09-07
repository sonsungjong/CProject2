// RealWinWiz.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Resource.h"
#include <shellapi.h>

// 변경할 함수 (메시지가 들어올 때마다 실행되는 함수)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{       // HWND 창의ID, uMsg 메시지명, wParam 정보1, lParam 정보2
    if (uMsg == WM_DESTROY) {   // WM_CLOSE 처리후에 들어오는 메시지
        PostQuitMessage(0);  // 자신의 메시지 큐에 WM_QUIT 메시지를 넣는다!
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        HWND h_wnd = FindWindow(L"Notepad", NULL);
        if (h_wnd != NULL) {
            PostMessage(h_wnd, WM_CLOSE, NULL, NULL);
        }
        ShellExecute(NULL, L"open", L"notepad", NULL, NULL, SW_SHOW);
        //ShellExecute(NULL, L"open", L"https://naver.com", NULL, NULL, SW_SHOW);
        //ShellExecute(NULL, L"open", L"C:\\Users\\user", NULL, NULL, SW_SHOW);
    }
    else if (uMsg == WM_MBUTTONDOWN) {
        HWND h_wnd = FindWindow(L"Notepad", NULL);
        if (h_wnd != NULL) {
            SetWindowText(h_wnd, L"파이팅!!");
        }
    }
    else if (uMsg == WM_RBUTTONDOWN) {
        HWND h_wnd = FindWindow(L"Notepad", NULL);          // 최상단Wnd 찾기
        if (h_wnd != NULL) {
            //SetWindowPos(h_wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            SetWindowPos(h_wnd, HWND_TOPMOST, 0, 0, 800, 800, 0);

            HWND h_child_wnd = FindWindowEx(h_wnd, NULL, L"Edit", NULL);            // 자식Wnd용
            if (h_child_wnd != NULL) {
                SetWindowPos(h_child_wnd, NULL, 50, 50, 150, 150, NULL);
                SendMessage(h_child_wnd, WM_SETTEXT, 0, (LPARAM)L"글방해!!");
                // 필터드라이버
            }
        }
    }

    // 개발자가 처리하지 않은 메시지들을 처리한다!
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Main 실행부
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
    wc.lpszClassName = L"tipssoft";    // 윈도우 클래스 이름 지정
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);    // 윈도우 클래스 등록

    // 윈도우 생성!
    HWND hWnd = CreateWindowW(L"tipssoft", L"www.tipssoft.com",
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
