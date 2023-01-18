// Gdiplus6IMG.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Gdiplus6IMG.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;
// #define WIN32_LEAN_AND_MEAN를 제거하기

int g_flag = 0;
int num1 = 0, num2 = 0, origin = 0, result = 0;

void NumBtn(HWND hWnd, int num)
{
    if (g_flag == -1) {
        SetDlgItemInt(hWnd, IDC_EDIT1, 0, TRUE);
        g_flag = 0;
    }
    origin = GetDlgItemInt(hWnd, IDC_EDIT1, NULL, TRUE);
    result = origin * 10 + num;
    SetDlgItemInt(hWnd, IDC_EDIT1, result, TRUE);
    origin = result;
}

void OperBtn(HWND hWnd, int flag)
{
    if (flag == 0) {
        // =
        num2 = GetDlgItemInt(hWnd, IDC_EDIT1, NULL, TRUE);
        if (g_flag == 1) {
            // +
            result = num1 + num2;
            SetDlgItemInt(hWnd, IDC_EDIT1, result, TRUE);
        }
        else if (g_flag == 2) {
            // -
            result = num1 - num2;
            SetDlgItemInt(hWnd, IDC_EDIT1, result, TRUE);
        }
        else if (g_flag == 3) {
            // ×
            result = num1 * num2;
            SetDlgItemInt(hWnd, IDC_EDIT1, result, TRUE);
        }
        else if (g_flag == 4) {
            // ÷
            if(num2 != 0){
                result = num1 / num2;
                SetDlgItemInt(hWnd, IDC_EDIT1, result, TRUE);
            }
            else {
                result = 0;
                SetDlgItemInt(hWnd, IDC_EDIT1, result, TRUE);
            }
        }
        g_flag = flag;
    }
    else if (flag == 1) {
        // +
        num1 = GetDlgItemInt(hWnd, IDC_EDIT1, NULL, TRUE);
        SetDlgItemInt(hWnd, IDC_EDIT1, 0, TRUE);
        g_flag = flag;

    }
    else if (flag == 2) {
        // -
        num1 = GetDlgItemInt(hWnd, IDC_EDIT1, NULL, TRUE);
        SetDlgItemInt(hWnd, IDC_EDIT1, 0, TRUE);
        g_flag = flag;
    }
    else if (flag == 3) {
        // ×
        num1 = GetDlgItemInt(hWnd, IDC_EDIT1, NULL, TRUE);
        SetDlgItemInt(hWnd, IDC_EDIT1, 0, TRUE);
        g_flag = flag;
    }
    else if (flag == 4) {
        // ÷
        num1 = GetDlgItemInt(hWnd, IDC_EDIT1, NULL, TRUE);
        SetDlgItemInt(hWnd, IDC_EDIT1, 0, TRUE);
        g_flag = flag;
    }
    else if (flag == 5) {
        num1 = 0, num2 = 0, origin = 0, result = 0;
        SetDlgItemInt(hWnd, IDC_EDIT1, 0, TRUE);
        g_flag = 0;
    }
}

LRESULT CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Gdiplus::Graphics gp(hdc);
        Image img = _T("../img/sea.jpg");
        gp.DrawImage(&img, 0, 0, 400, 330);
        EndPaint(hWnd, &ps);
    }
    return (LRESULT)TRUE;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BUTTON1:
            NumBtn(hWnd,1);
            break;
        case IDC_BUTTON2:
            NumBtn(hWnd, 2);
            break;
        case IDC_BUTTON3:
            NumBtn(hWnd, 3);
            break;
        case IDC_BUTTON4:
            NumBtn(hWnd, 4);
            break;
        case IDC_BUTTON5:
            NumBtn(hWnd, 5);
            break;
        case IDC_BUTTON6:
            NumBtn(hWnd, 6);
            break;
        case IDC_BUTTON7:
            NumBtn(hWnd, 7);
            break;
        case IDC_BUTTON8:
            NumBtn(hWnd, 8);
            break;
        case IDC_BUTTON9:
            NumBtn(hWnd, 9);
            break;
        case IDC_BUTTON10:
            NumBtn(hWnd, 0);
            break;
        case IDC_BUTTON11:
            OperBtn(hWnd, 0);           // =
            break;
        case IDC_BUTTON12:
            OperBtn(hWnd, 1);       // +
            break;
        case IDC_BUTTON13:
            OperBtn(hWnd, 2);           // -
            break;
        case IDC_BUTTON14:
            OperBtn(hWnd, 3);           // ×
            break;
        case IDC_BUTTON15:
            OperBtn(hWnd, 4);           // ÷
            break;
        case IDC_BUTTON16:
            OperBtn(hWnd, 5);           // clear
            break;
        case IDCANCEL:
            EndDialog(hWnd, LOWORD(wParam));
            break;
        }
    }
    return (LRESULT)TRUE;
    case WM_INITDIALOG:
        //SetWindowPos(hWnd, HWND_TOP, 0, 0, 500, 500, NULL);

        return (LRESULT)TRUE;
    case WM_CREATE:

        return TRUE;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    }
    return (LRESULT)FALSE;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // Gdi+ 초기화작업
    ULONG_PTR gpToken;
    GdiplusStartupInput gpsi;
    if (GdiplusStartup(&gpToken, &gpsi, NULL) != Ok) {
        return 0;
    }


    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DlgProc);

    // Gdi+ 정리작업
    GdiplusShutdown(gpToken);


    return 0;
}
