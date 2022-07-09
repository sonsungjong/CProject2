#include "framework.h"
#include "OpenCVDlg.h"

LRESULT CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static RECT s_rect;
    
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        
        EndPaint(hWnd, &ps);
    }
    return (LRESULT)TRUE;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
            
        }
    }
    return (LRESULT)TRUE;
    case WM_INITDIALOG:
        //SetWindowPos(hWnd, HWND_TOP, 100, 100, 475, 500, NULL);
        //GetWindowRect(IDC_PICTURE1,&s_rect);
        return (LRESULT)TRUE;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    }
    return (LRESULT)FALSE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DlgProc);

    return 0;
}