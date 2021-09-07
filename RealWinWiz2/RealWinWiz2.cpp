// RealWinWiz2.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include <shellapi.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY) {   // WM_CLOSE ó���Ŀ� ������ �޽���
        PostQuitMessage(0);  // �ڽ��� �޽��� ť�� WM_QUIT �޽����� �ִ´�!
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
            SetWindowText(h_wnd_fw, L"��Ʈ�е� ���� �ٲٱ�");
            SendMessage(h_wnd_fwe, WM_SETTEXT, 0, (LPARAM)L"�۹����ϱ�~~");
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

    // �����ڰ� ó������ ���� �޽������� ó���Ѵ�!
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

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
    wc.lpszClassName = L"��Ʈ�е�����";    // ������ Ŭ���� �̸� ����
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);    // ������ Ŭ���� ���

    // ������ ����!
    HWND hWnd = CreateWindowW(L"��Ʈ�е�����", L"��Ʈ�е� ���� �θ���~",
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