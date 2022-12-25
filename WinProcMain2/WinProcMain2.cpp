#include "framework.h"
#include "WinProcMain2.h"

template<class DERIVED_TYPE>
class BaseWindow
{
public:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE* pThis = NULL;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hWnd = hWnd;
        }
        else
        {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }

    explicit BaseWindow() : m_hWnd(NULL) { }
    virtual ~BaseWindow() { DeleteObject(m_hBrush); }

    BOOL Create(
        PCTSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        WNDCLASS wc = { 0 };

        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hbrBackground = m_hBrush;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.lpfnWndProc = DERIVED_TYPE::WndProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();
        wc.lpszMenuName = NULL;
        wc.style = CS_HREDRAW | CS_VREDRAW;

        RegisterClass(&wc);

        m_hWnd = CreateWindowEx(
            dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
        );
        return (m_hWnd ? TRUE : FALSE);
    }
    
    HWND Window() const { return m_hWnd; }

protected:
    virtual PCTSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    
    HWND m_hWnd = NULL;

    HBRUSH m_hBrush = CreateSolidBrush(RGB(255, 255, 255));
};

class MainWindow : public BaseWindow<MainWindow>
{
public:
    PCTSTR  ClassName() const { return _T("Sample Window Class"); }
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MainWindow win;

    if (!win.Create(_T("Window Class"), WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    // TODO : Initialize

    ShowWindow(win.Window(), nCmdShow);
    UpdateWindow(win.Window());

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT MainWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    if (msg == WM_PAINT) {
        hdc = BeginPaint(m_hWnd, &ps);
        // TODO:

        EndPaint(m_hWnd, &ps);
        return 0;
    }
    else if (msg == WM_COMMAND) {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(m_hWnd);
            break;
        default:
            return DefWindowProc(m_hWnd, msg, wParam, lParam);
        }
        return 0;
    }
    else if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    
    return DefWindowProc(m_hWnd, msg, wParam, lParam);
}
