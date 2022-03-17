// WinStep35ListView1.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "WinStep35ListView1.h"
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")                    // InitCommonControls()

HINSTANCE hInst;
INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
    return 0;
}

typedef struct {
    TCHAR name[20];
    TCHAR dept[20];
    TCHAR address[60];
    BOOL male;
} STUDENT;

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hList;
    STUDENT students[] = {
        {_T("��ö��"), _T("��ǻ�Ͱ���"), _T("�����"), TRUE},
        {_T("���μ�"), _T("������"), _T("�λ��"), TRUE},
        {_T("�̿���"), _T("������"), _T("������"), FALSE},
    };
    if (msg == WM_CLOSE) {
        //DestroyWindow(hDlg);
        PostQuitMessage(0);
        return (INT_PTR)TRUE;
    }
    else if (msg == WM_DESTROY) {
        return (INT_PTR)TRUE;
    }
    else if (msg == WM_INITDIALOG) {
        InitCommonControls();               // ���� ��Ʈ�� DLL �ε� Ȯ��
        hList = GetDlgItem(hDlg, IDC_LIST2);
        RECT rect;
        GetClientRect(hList, &rect);
        int nWidth = rect.right - rect.left;

        // �÷� ���
        LVCOLUMN col;
        ZeroMemory(&col, sizeof(LVCOLUMN));
        col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
        col.fmt = LVCFMT_CENTER;

        col.pszText = (LPTSTR)_T("����");
        col.cx = 50;
        ListView_InsertColumn(hList, 0, &col);

        col.pszText = (LPTSTR)_T("�̸�");
        col.cx = 120;
        ListView_InsertColumn(hList, 1, &col);
        
        col.pszText = (LPTSTR)_T("�а�");
        col.cx = 130;
        ListView_InsertColumn(hList, 2, &col);

        col.cx = nWidth - 300;
        col.pszText = (LPTSTR)_T("�ּ�");
        ListView_InsertColumn(hList, 3, &col);

        // �׸� �߰�
        LVITEM row;
        ZeroMemory(&row, sizeof(LVITEM));
        row.mask = LVIF_TEXT;
        for (int i = 0; i < 3; i++) {
            row.iItem = i;
            row.iSubItem = 0;
            row.pszText = (LPTSTR)(students[i].male ? _T("����") : _T("����"));
            ListView_InsertItem(hList, &row);

            ListView_SetItemText(hList, i, 1, students[i].name);
            ListView_SetItemText(hList, i, 2, students[i].dept);
            ListView_SetItemText(hList, i, 3, students[i].address);
        }
        // �� ��ü ���� ����
        ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}
