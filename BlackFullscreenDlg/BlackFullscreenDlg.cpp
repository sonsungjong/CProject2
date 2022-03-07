
#include "framework.h"
#include "BlackFullscreenDlg.h"


// Global Variables:
HINSTANCE hInst;                                // current instance

INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DlgProc);

    return 0;
}


INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static RECT rect;
    static int x, y;
    switch (msg)
    {
    case WM_INITDIALOG:
        x = GetSystemMetrics(SM_CXSCREEN);
        y = GetSystemMetrics(SM_CYSCREEN);
        SetWindowPos(hDlg, HWND_TOP, 0, 0, x, y, NULL);
        GetClientRect(hDlg, &rect);
        return (INT_PTR)TRUE;
    case WM_CREATE:
        return (INT_PTR)TRUE;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        SetRect(&rect, 0, 0, rect.right, rect.bottom);
        FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
        EndPaint(hDlg, &ps);
    }
    return (INT_PTR)TRUE;
    case WM_COMMAND:
        //if (LOWORD(wParam) == IDCANCEL)
        //{
        //    EndDialog(hDlg, LOWORD(wParam));
        //    return (INT_PTR)TRUE;
        //}
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        DestroyWindow(hDlg);
        break;
    }
    return (INT_PTR)FALSE;
}
