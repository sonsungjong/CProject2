// NotePadEx.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "NotePadEx.h"

#define MAX_LOADSTRING 100
#define MAXSIZE 512
#define MAX_STRING 8192

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING] = _T("SJNote");                  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING] = _T("SJNote");            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    //LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadString(hInstance, IDC_NOTEPADEX, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTEPADEX));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOTEPADEX));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NOTEPADEX);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 720, 800, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int total_num = 2000;
    static int prepayment = 1000;
    static int bus_prevention = 500;
    static HWND hEdit, hBus, hPrePay, hTotal, hStaticTotal, hStaticPrePay, hStaticBus;
    static TCHAR* pszBuf;
    static int cxClient, cyClient;
    cxClient = LOWORD(lParam);
    cyClient = HIWORD(lParam);
    static TCHAR lpstrFile[MAXSIZE] = _T("");
    static TCHAR lpstrFileTitle[MAXSIZE] = _T("");
    static OPENFILENAME ofn;
    static COLORREF textColor;
    static LOGFONT lf = { 50, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, _T("굴림") };
    switch (message)
    {
    case WM_CREATE:
        hStaticTotal = CreateWindow(_T("STATIC"), _T("총 개수"), WS_CHILD | WS_VISIBLE | WS_BORDER|ES_CENTER, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hWnd, (HMENU)IDC_STATIC_TOTAL, hInst, nullptr);
        hStaticPrePay = CreateWindow(_T("STATIC"), _T("선입금"), WS_CHILD | WS_VISIBLE | WS_BORDER|ES_CENTER, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hWnd, (HMENU)IDC_STATIC_PREPAY, hInst, nullptr);
        hStaticBus = CreateWindow(_T("STATIC"), _T("버스방지"), WS_CHILD | WS_VISIBLE | WS_BORDER|ES_CENTER, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hWnd, (HMENU)IDC_STATIC_BUS, hInst, nullptr);
        hTotal = CreateWindow(_T("EDIT"), NULL, WS_CHILD|WS_VISIBLE|WS_BORDER|ES_NUMBER|ES_READONLY|ES_CENTER, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hWnd, (HMENU)IDC_TOTAL, hInst, nullptr);
        hPrePay = CreateWindow(_T("EDIT"), NULL, WS_CHILD|WS_VISIBLE|WS_BORDER|ES_NUMBER|ES_READONLY|ES_CENTER, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hWnd, (HMENU)IDC_PREPAY, hInst, nullptr);
        hBus = CreateWindow(_T("EDIT"), NULL, WS_CHILD|WS_VISIBLE|WS_BORDER|ES_NUMBER|ES_READONLY|ES_CENTER, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hWnd, (HMENU)IDC_BUS, hInst, nullptr);
        hEdit = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | WS_BORDER, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hWnd, (HMENU)IDC_EDIT, hInst, nullptr);
        SetFocus(hEdit);
        pszBuf = new TCHAR[MAX_STRING];
        SetDlgItemInt(hWnd, IDC_TOTAL, total_num, FALSE);
        SetDlgItemInt(hWnd, IDC_PREPAY, prepayment, FALSE);
        SetDlgItemInt(hWnd, IDC_BUS, bus_prevention, FALSE);

        ZeroMemory(&ofn, 0, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hWnd;
        ofn.hInstance = hInst;
        ofn.lpstrFilter = _T("텍스트 파일(*.txt)\0*.txt\0") _T("모든 파일(*.*)\0 *.*\0\0");
        ofn.lpstrFile = lpstrFile;
        ofn.lpstrDefExt = _T("txt");
        ofn.nMaxFile = MAXSIZE;
        ofn.lpstrFileTitle = lpstrFileTitle;
        ofn.nMaxFileTitle = MAXSIZE;
        break;
    case WM_SIZE:
        //cxClient = LOWORD(lParam);
        //cyClient = HIWORD(lParam);
        MoveWindow(hStaticTotal, 0, cyClient*41/100, cxClient *3/ 20, cyClient * 44 / 100, FALSE);
        MoveWindow(hStaticPrePay, 0, cyClient*44/100, cxClient *3/ 20, cyClient * 47 / 100, FALSE);
        MoveWindow(hStaticBus, 0, cyClient*47/100, cxClient *3/ 20, cyClient /2, FALSE);
        MoveWindow(hTotal, cxClient*3/20, cyClient*41/100, cxClient*3/20, cyClient*44/100, FALSE);
        MoveWindow(hPrePay, cxClient * 3 / 20, cyClient*44/100, cxClient*3/20, cyClient*47/100, FALSE);
        MoveWindow(hBus, cxClient * 3 / 20, cyClient*47/100, cxClient*3/20, cyClient/2, FALSE);
        MoveWindow(hEdit, 0, cyClient / 2, cxClient, cyClient/2, FALSE);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
                case IDC_EDIT:
                    if (HIWORD(wParam) == EN_CHANGE) {
                        GetWindowText(hEdit, pszBuf, MAX_STRING);
                    }
                    break;
                case IDC_BUS:
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                case IDM_FILE_OPEN:
                    ofn.Flags = OFN_FILEMUSTEXIST;
                    if (GetOpenFileName(&ofn)) {
                        MessageBox(hWnd, ofn.lpstrFile, _T("파일 열기"), MB_OK);
                    }
                    break;
                case IDM_FILE_SAVE:
                    MessageBox(hWnd, ofn.lpstrFile, _T("파일 저장"), MB_OK);
                    break;
                case IDM_FILE_SAVE_AS:
                    ofn.Flags = OFN_OVERWRITEPROMPT;
                    if (GetSaveFileName(&ofn)) {
                        MessageBox(hWnd, ofn.lpstrFile, _T("다른 이름으로 저장"), MB_OK);
                        EnableMenuItem(GetMenu(hWnd), IDM_FILE_SAVE, MF_ENABLED);
                    }
                    break;
                case IDM_OPTION_FONT:
                {
                    CHOOSEFONT cf;
                    static TCHAR szStyle[] = _T("보통");
                    memset(&cf, 0, sizeof(CHOOSEFONT));
                    cf.lStructSize = sizeof(CHOOSEFONT);
                    cf.hwndOwner = hWnd;
                    cf.Flags = CF_EFFECTS | CF_SCREENFONTS | CF_USESTYLE | CF_INITTOLOGFONTSTRUCT;
                    cf.lpszStyle = szStyle;
                    cf.lpLogFont = &lf;
                    if (ChooseFont(&cf))
                    {
                        textColor = cf.rgbColors;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                }
                break;
                case IDM_ABOUT:
                    PlaySound(TEXT("C:\\sound2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    PlaySound(NULL, 0, 0);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        delete[] pszBuf;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
        break;
    }
    return (INT_PTR)FALSE;
}
