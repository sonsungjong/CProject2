// Calculator.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Calculator.h"

HINSTANCE hInst;                                // current instance
int num1 = -1, num2 = 0, current_num = 0;
int cal = 0, reset_flag = 0;

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_CAL) {
            if (cal == 1) {
                SetDlgItemInt(hDlg, IDC_SHOW, num1 + num2, TRUE);
                num1 = -1;
                num2 = 0;
                cal = 0;
                reset_flag = 1;
            }
            else if (cal == 2) {
                SetDlgItemInt(hDlg, IDC_SHOW, num1 - num2, TRUE);
                num1 = -1;
                num2 = 0;
                cal = 0;
                reset_flag = 1;
            }
            else if (cal == 3) {
                SetDlgItemInt(hDlg, IDC_SHOW, num1 * num2, TRUE);
                num1 = -1;
                num2 = 0;
                cal = 0;
                reset_flag = 1;
            }
            else if (cal == 4 && num2 != 0) {
                SetDlgItemInt(hDlg, IDC_SHOW, num1 / num2, TRUE);
                num1 = -1;
                num2 = 0;
                cal = 0;
                reset_flag = 1;
            }
        }
        switch (LOWORD(wParam))
        {
        case IDC_NUM0:
            InputNum(0, hDlg);
            break;
        case IDC_NUM1:
            InputNum(1, hDlg);
            break;
        case IDC_NUM2:
            InputNum(2, hDlg);
            break;
        case IDC_NUM3:
            InputNum(3, hDlg);
            break;
        case IDC_NUM4:
            InputNum(4, hDlg);
            break;
        case IDC_NUM5:
            InputNum(5, hDlg);
            break;
        case IDC_NUM6:
            InputNum(6, hDlg);
            break;
        case IDC_NUM7:
            InputNum(7, hDlg);
            break;
        case IDC_NUM8:
            InputNum(8, hDlg);
            break;
        case IDC_NUM9:
            InputNum(9, hDlg);
            break;
        case IDC_PLUS:
            reset_flag = 1;
            cal = 1;
            break;
        case IDC_MINUS:
            reset_flag = 1;
            cal = 2;
            break;
        case IDC_MUL:
            reset_flag = 1;
            cal = 3;
            break;
        case IDC_DIV:
            reset_flag = 1;
            cal = 4;
            break;
        case IDC_CLEAR:
            SetDlgItemInt(hDlg, IDC_SHOW, 0, TRUE);
            num1 = -1, num2 = 0, current_num = 0;
            cal = 0, reset_flag = 0;
        case IDC_BACK:
            int origin = GetDlgItemInt(hDlg, IDC_SHOW, NULL, TRUE);
            SetDlgItemInt(hDlg, IDC_SHOW, origin / 10, TRUE);
        }
        break;
    }
    return (INT_PTR)FALSE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
    return 0;
}


void InputNum(int input_num, HWND hDlg)
{
    if (reset_flag != 0) {
        reset_flag = 0;
        SetDlgItemInt(hDlg, IDC_SHOW, 0, TRUE);
    }
    current_num = GetDlgItemInt(hDlg, IDC_SHOW, NULL, TRUE);

    SetDlgItemInt(hDlg, IDC_SHOW, current_num * 10 + input_num, TRUE);
    if (cal != 0) {
        num2 = current_num * 10 + input_num;
    }
    else {
        num1 = current_num * 10 + input_num;
    }
}