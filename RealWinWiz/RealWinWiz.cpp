// RealWinWiz.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Resource.h"
#include <shellapi.h>

// ������ �Լ� (�޽����� ���� ������ ����Ǵ� �Լ�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{       // HWND â��ID, uMsg �޽�����, wParam ����1, lParam ����2
    if (uMsg == WM_DESTROY) {   // WM_CLOSE ó���Ŀ� ������ �޽���
        PostQuitMessage(0);  // �ڽ��� �޽��� ť�� WM_QUIT �޽����� �ִ´�!
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
            SetWindowText(h_wnd, L"������!!");
        }
    }
    else if (uMsg == WM_RBUTTONDOWN) {
        HWND h_wnd = FindWindow(L"Notepad", NULL);          // �ֻ��Wnd ã��
        if (h_wnd != NULL) {
            //SetWindowPos(h_wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            SetWindowPos(h_wnd, HWND_TOPMOST, 0, 0, 800, 800, 0);

            HWND h_child_wnd = FindWindowEx(h_wnd, NULL, L"Edit", NULL);            // �ڽ�Wnd��
            if (h_child_wnd != NULL) {
                SetWindowPos(h_child_wnd, NULL, 50, 50, 150, 150, NULL);
                SendMessage(h_child_wnd, WM_SETTEXT, 0, (LPARAM)L"�۹���!!");
                // ���͵���̹�
            }
        }
    }

    // �����ڰ� ó������ ���� �޽������� ó���Ѵ�!
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Main �����
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ������ Ŭ���� ��� �۾�!!
    WNDCLASS wc;

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;     // ���� ����
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);                // Ŀ�� ����
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // ���� ���α׷� �ΰ� ������ ����
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;    // �⺻ �޽��� ó���� �Լ� ����
    wc.lpszClassName = L"tipssoft";    // ������ Ŭ���� �̸� ����
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);    // ������ Ŭ���� ���

    // ������ ����!
    HWND hWnd = CreateWindowW(L"tipssoft", L"www.tipssoft.com",
        WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);   // �����츦 ȭ�鿡 ��� ����� ���ΰ� ����!
    UpdateWindow(hWnd);    // WM_PAINT �޽����� ��� ���¿� �ִٸ� ��� �����ϵ��� �Ѵ�!

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {   // WM_QUIT �޽����� �߻��Ҷ����� �ݺ��Ѵ�.
        TranslateMessage(&msg);   // Ű���� �޽��� �߻��ÿ� �߰� �޽����� �߻����� ���� üũ
        DispatchMessage(&msg);   // �߻��� �޽����� ó���Ѵ�!
    }
    return msg.wParam;
}
