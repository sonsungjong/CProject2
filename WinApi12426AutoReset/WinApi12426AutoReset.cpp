// WinApi12426AutoReset.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WinApi12426AutoReset.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

const TCHAR* lpszEvent = _T("Event1");              // 이벤트 객체이름
HWND hMainWnd;
DWORD_PTR WINAPI ThreadProc(void* lpParameter);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPI12426AUTORESET, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI12426AUTORESET));

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
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI12426AUTORESET));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPI12426AUTORESET);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
    static int nCount = 1;
    static HANDLE hEvent;
    switch (message)
    {
    case WM_CREATE:
    {
        hMainWnd = hWnd;
        int bManualReset = FALSE;                  // 자동리셋
        int bInitialState = FALSE;                      // 비신호 상태
        hEvent = CreateEvent(nullptr, bManualReset, bInitialState, lpszEvent);
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_THREAD_CREATE:
            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ThreadProc, (void*)nCount++, 0,0);
            break;
        case IDM_SET_EVENT:
            // hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEvent);
            SetEvent(hEvent);
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
        CloseHandle(hEvent);
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

DWORD_PTR WINAPI ThreadProc(void* lpParameter)
{
    TCHAR szBuffer[128];
    int nCount = (int)lpParameter;

    // 이벤트 객체의 핸들을 얻어온다.
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEvent);
    HDC hdc = GetDC(hMainWnd);
    _stprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), _T("Thread ID=%d is waiting"), GetCurrentThreadId());
    TextOut(hdc, 10, 20 * nCount, szBuffer, _tcslen(szBuffer));
    GdiFlush();

    // 이벤트 객체가 신호 상태가 되기를 기다림
    WaitForSingleObject(hEvent, INFINITE);
    for (int i = 0; i < 2000; i++) {
        Sleep(1);
        _stprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), _T("Thread ID=%d is in Event, nCount=%d: i=%d"), GetCurrentThreadId(), nCount, i);
        TextOut(hdc, 10, 20 * nCount, szBuffer, _tcslen(szBuffer));
    }
    GdiFlush();

    SetEvent(hEvent);

    memset(szBuffer, 0, sizeof(szBuffer));
    _stprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), _T("Thread ID=%d is terminated!,""                             "), GetCurrentThreadId());

    TextOut(hdc, 10, 20 * nCount, szBuffer, _tcslen(szBuffer));
    GdiFlush();
    ReleaseDC(hMainWnd, hdc);
    CloseHandle(hEvent);
    return nCount;
}