#include "framework.h"

#include "BaseWindow.h"
#include "WinProcMain3.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MainWindow win;

    if (!win.Create(_T("My Circle"), WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }
    // TODO : Initialize


    ShowWindow(win.Window(), nCmdShow);
    UpdateWindow(win.Window());


    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
