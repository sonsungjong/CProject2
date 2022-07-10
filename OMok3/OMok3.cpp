// OMok3.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "OMok3.h"


#define X_COUNT         19                  // ������ ����
#define Y_COUNT         19                      // ������ ����
#define START_X         50                  // x�� ����
#define START_Y         50                  // y�� ����
#define INTERVAL        26                  // �� ���� ����, �ٵϵ� ����
#define HALF_INTERVAL       INTERVAL/2                  // �ٵϵ� ������
#define XPOS(x) (START_X + (x) * INTERVAL)              // �ٵϵ��� ���� ��ġ
#define YPOS(y) (START_Y + (y) * INTERVAL)              // �ٵϵ��� ���� ��ġ

unsigned char g_dol[Y_COUNT][X_COUNT];              // ��ġ���� �� ���� // 0 : ������, 1: ������, 2: ��
unsigned char g_step;                                               // 0�̸� ������ ���� ����, 1�̸� ��� ���� ����

LRESULT CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH hBrush;
    int a_x = LOWORD(lParam);
    int a_y = HIWORD(lParam);
    PAINTSTRUCT ps;
    HDC hdc;

    switch (msg)
    {
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        //GdiplusDrawImage(hdc);
        for (int x = 0; x < X_COUNT; x++) {
            MoveToEx(hdc, XPOS(x), YPOS(0), NULL);
            LineTo(hdc, XPOS(x), YPOS(Y_COUNT - 1));
        }

        for (int y = 0; y < Y_COUNT; y++) {
            MoveToEx(hdc, XPOS(0), YPOS(y), NULL);
            LineTo(hdc, XPOS(X_COUNT - 1), YPOS(y));
        }

        for (int y = 0; y < Y_COUNT; y++) {
            for (int x = 0; x < X_COUNT; x++) {
                if (g_dol[y][x] > 0) {
                    if (g_dol[y][x] == 1) {
                        SelectObject(hdc, GetStockObject(BLACK_BRUSH));
                    }
                    else {
                        SelectObject(hdc, GetStockObject(WHITE_BRUSH));
                    }
                    Ellipse(hdc, XPOS(x) - HALF_INTERVAL, YPOS(y) - HALF_INTERVAL, XPOS(x) + HALF_INTERVAL, YPOS(y) + HALF_INTERVAL);
                }
            }
        }
        EndPaint(hWnd, &ps);
    }
    return (LRESULT)TRUE;
    case WM_CTLCOLORDLG:
        return (INT_PTR)hBrush;
    case WM_LBUTTONDOWN: 
        if (a_x > (XPOS(0) - HALF_INTERVAL) && a_y > (YPOS(0) - HALF_INTERVAL) && a_x < (XPOS(X_COUNT - 1) + HALF_INTERVAL) && a_y < (YPOS(Y_COUNT - 1) + HALF_INTERVAL)) {
            int x = (a_x - START_X + HALF_INTERVAL) / INTERVAL;
            int y = (a_y - START_Y + HALF_INTERVAL) / INTERVAL;
            if (g_dol[y][x] == 0) {
                g_dol[y][x] = g_step + 1;
                g_step = !g_step;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDCANCEL:
            EndDialog(hWnd, LOWORD(wParam));
            break;
        }
    }
    return (LRESULT)TRUE;
    case WM_INITDIALOG:
        SetWindowPos(hWnd, HWND_TOP, 50, 50, 600, 600, NULL);
        hBrush = CreateSolidBrush(RGB(244, 176, 77));
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

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{


    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DlgProc);

    return 0;
}
