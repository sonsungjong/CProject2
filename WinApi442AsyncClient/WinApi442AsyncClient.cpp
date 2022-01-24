// WinApi442AsyncClient.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WinApi442AsyncClient.h"

#define MAX_LOADSTRING 100
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// Global Variables:
HINSTANCE hInst;                                // current instance
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DlgProc);
    return 0;
}

#define WM_CLIENT WM_USER+2
#define BUFSIZE 512
typedef enum { LOGIN = 1, LOGEXIST, LOGOUT } PACKET_TYPE;
typedef struct {
    PACKET_TYPE ptype;
    unsigned short len;
    char data[BUFSIZE];
}PACKET;


INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static SOCKET client;
    static SOCKADDR_IN addr;
    PACKET packet;
    static TCHAR loginID[BUFSIZE];
    static int nTrial = 10;
    static BOOL bLogin = FALSE, bConnected = FALSE;
    switch (message)
    {
        case WM_INITDIALOG:
        {
            //WinSock2 초기화
            WSADATA wsaData;
            WSAStartup(0x0202, &wsaData);
            ZeroMemory(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = 10000;

            // 서버가 다른 컴퓨터에 있으면 IP주소를 변경해야 함
            inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
            SetTimer(hDlg, 1, 1000, nullptr);
            SendMessage(hDlg, WM_TIMER, 1, 0);
            return (INT_PTR)TRUE;
        }
        case WM_TIMER:
            nTrial--;               // 시도횟수 감소
            // IPv4, TCP stream, protocol
            client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (connect(client, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
            {
                closesocket(client);
                bConnected = FALSE;
            }else {                         // connection OK
                bConnected = TRUE;
                WSAAsyncSelect(client, hDlg, WM_CLIENT, FD_READ | FD_CLOSE);
                SetWindowText(hDlg, _T("Connected..."));
                KillTimer(hDlg, 1);
            }
            if (nTrial == 0)
            {
                KillTimer(hDlg, 1);
            }
            return (INT_PTR)TRUE;

            // wParam: socket, LOWORD(lParam): network event, HIWORD(lParam): error code
        case WM_CLIENT:
            switch (WSAGETSELECTEVENT(lParam))
            {
                case FD_CLOSE:                                      // 소켓이 닫힐 때
                    closesocket((SOCKET)wParam);                // 클라이언트 소켓을 닫음
                    bLogin = FALSE;
                    bConnected = FALSE;
                    EnableWindow(GetDlgItem(hDlg, IDC_EDIT_LOGIN), TRUE);
                    SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("로그인"));
                    SetWindowText(hDlg, _T("Disconnected..."));

                    nTrial = 20;                // 자동 연결 시도
                    SetTimer(hDlg, 1, 1000, nullptr);
                    SendMessage(hDlg, WM_TIMER, 1, 0);
                    break;
                case FD_READ:
                {
                    // client = (SOCKET)wParam;
                    int nR = recv(client, (char*)&packet, sizeof(PACKET), 0);
                    switch (packet.ptype)
                    {
                        case LOGIN:
                            bLogin = TRUE;
                            memcpy(loginID, packet.data, packet.len);
                            SetWindowText(hDlg, _T("환영합니다."));
                            EnableWindow(GetDlgItem(hDlg, IDC_EDIT_LOGIN), FALSE);
                            SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("로그아웃"));
                            break;
                        case LOGOUT:
                            bLogin = FALSE;
                            memset(loginID, 0, BUFSIZE);
                            SetWindowText(hDlg, _T("로그인이 필요합니다."));
                            EnableWindow(GetDlgItem(hDlg, IDC_EDIT_LOGIN), TRUE);
                            SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("로그인"));
                            break;
                        case LOGEXIST:
                            bLogin = FALSE;
                            SetWindowText(hDlg, _T("서버에 같은 아이디가 존재합니다."));
                            break;
                    }
                    break;
                }
            }
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDC_BUTTON_LOGIN:
                    if (!bConnected) { break; }
                    if (bLogin)             // Log out
                    {
                        packet.ptype = LOGOUT;
                        packet.len = (unsigned short)(_tcslen(loginID) + 1) * sizeof(TCHAR);
                        memcpy(packet.data, loginID, packet.len);
                    }else {                 // Log in
                        TCHAR strID[BUFSIZE];
                        GetDlgItemText(hDlg, IDC_EDIT_LOGIN, strID, 128);
                        if (_tcslen(strID) == 0) { break; }
                        packet.ptype = LOGIN;
                        packet.len = (unsigned short)(_tcslen(strID) + 1) * sizeof(TCHAR);
                        memcpy(packet.data, strID, packet.len);
                    }
                    send(client, (char*)&packet, sizeof(PACKET), 0);
                    break;
            }
            return (INT_PTR)TRUE;
        case WM_CLOSE:
            DestroyWindow(hDlg);
            return TRUE;
        case WM_DESTROY:
            closesocket(client);
            WSACleanup();
            return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}