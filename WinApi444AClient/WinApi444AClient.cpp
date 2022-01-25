// WinApi444AClient.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "WinApi444AClient.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// Global Variables:
HINSTANCE hInst;                                // current instance
#define PORT 10000
#define IP  "127.0.0.1"

INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProcLogin(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DlgProc);

    return 0;
}

// 대화상자 메시지 처리기
#define WM_CLIENT WM_USER+1
#define WM_CONNECT WM_USER+2
#define BUFSIZE 512
typedef enum {
    LOGIN = 1, LOGEXIST, LOGOUT, OTHERLOGIN, OTHERLOGOUT, TALK
}PACKET_TYPE;

typedef struct {
    PACKET_TYPE ptype;
    unsigned short len;         // length(data)
    char data[BUFSIZE];
} PACKET;

TCHAR loginID[BUFSIZE];
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hList;
    static SOCKET client;
    static SOCKADDR_IN addr;
    PACKET packet;
    static bool bConnected = false;             // 서버 연결
    static bool bLogin = false;                 // 로그인
    switch (message)
    {
        case WM_INITDIALOG:
        {
            hList = GetDlgItem(hDlg, IDC_LIST1);
            // Winsock2.2 초기화
            WSADATA wsaData;
            WSAStartup(0x0202, &wsaData);

            ZeroMemory(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = 10000;
            // 서버가 다른 컴퓨터라면 IP를 변경할 것
            inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
            SendMessage(hDlg, WM_CONNECT, 0, 0);
            return (INT_PTR)TRUE;
        }
        case WM_CONNECT:
        {
            // IPv4, TCP stream, protocol
            client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (connect(client, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) {
                closesocket(client);
                bConnected = false;
                SetWindowText(hDlg, _T("Disconnected..."));
                SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), _T("서버 연결"));
            }
            else {              // 연결 완료
                bConnected = true;
                WSAAsyncSelect(client, hDlg, WM_CLIENT, FD_READ | FD_CLOSE);
                SetWindowText(hDlg, _T("Connected..."));
                SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), _T("서버 나가기"));
            }
            return (INT_PTR)TRUE;
        }
        // wParam : socket, LOWORD(lParam): network event
        case WM_CLIENT:
            switch (WSAGETSELECTEVENT(lParam))
            {
                case FD_CLOSE:                  // 소켓 닫힐 때
                    closesocket((SOCKET)wParam);
                    bLogin = false;
                    bConnected = false;

                    SetWindowText(hDlg, _T("Disconnected..."));
                    SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("로그인"));
                    SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), _T("서버 연결"));
                    SendMessage(hList, LB_RESETCONTENT, 0, 0);
                    break;
                case FD_READ:
                {
                    // client = (SOCKET)wParam;
                    TCHAR recvID[BUFSIZE];
                    TCHAR strTemp[BUFSIZE];
                    int nR = recv(client, (char*)&packet, sizeof(PACKET), 0);
                    switch (packet.ptype)
                    {
                    case LOGIN:
                        bLogin = true;
                        memcpy(loginID, packet.data, packet.len);
                        _stprintf_s(strTemp, sizeof(strTemp) / sizeof(TCHAR), _T("[%s]님 환영합니다."), loginID);
                        SetWindowText(hDlg, strTemp);
                        SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("로그 아웃"));
                        break;
                    case LOGOUT:
                    {
                        bLogin = false;
                        memset(loginID, 0, BUFSIZE);
                        SetWindowText(hDlg, _T("로그인이 필요합니다."));
                        SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("로그인"));
                        SendMessage(hList, LB_RESETCONTENT, 0, 0);
                        break;
                    }
                    case LOGEXIST:
                        bLogin = false;
                        SetWindowText(hDlg, _T("서버에 같은 아이디가 존재합니다."));
                        break;
                    case OTHERLOGIN:                    // IDC_LIST1에 추가
                    {
                        memcpy(recvID, packet.data, packet.len);
                        SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                        break;
                    }
                    case OTHERLOGOUT:                       // IDC_LIST1에서 삭제
                        memcpy(recvID, packet.data, packet.len);
                        unsigned int k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)recvID);
                        SendMessage(hList, LB_DELETESTRING, k, 0);
                        break;
                    }
                    break;
                }
            }
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case IDC_BUTTON_CONNECT:
                if (!bConnected) { SendMessage(hDlg, WM_CONNECT, 0, 0); }
                else {
                    closesocket(client);
                    SendMessage(hDlg, WM_CLIENT, 0, MAKEWORD(FD_CLOSE, 0));
                }
                break;
            case IDC_BUTTON_LOGIN:
                if (!bConnected) { break; }
                if (!bLogin) {
                    if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, DlgProcLogin) == IDOK)
                    {
                        if (_tcslen(loginID) == 0) { break; }
                        packet.ptype = LOGIN;
                        packet.len = (unsigned short)(_tcslen(loginID) + 1) * sizeof(TCHAR);
                        memcpy(packet.data, loginID, packet.len);
                        send(client, (char*)&packet, sizeof(PACKET), 0);
                    }
                }
                else {
                    packet.ptype = LOGOUT;
                    packet.len = (unsigned short)(_tcslen(loginID) + 1) * sizeof(TCHAR);
                    memcpy(packet.data, loginID, packet.len);
                    send(client, (char*)&packet, sizeof(PACKET), 0);
                }
                return (INT_PTR)TRUE;
            }
            return (INT_PTR)TRUE;
        case WM_CLOSE:
            if (bLogin)
            {
                packet.ptype = LOGOUT;
                packet.len = (unsigned short)(_tcslen(loginID) + 1) * sizeof(TCHAR);
                memcpy(packet.data, loginID, packet.len);
                send(client, (char*)&packet, sizeof(PACKET), 0);
            }
            DestroyWindow(hDlg);
            return TRUE;
        case WM_DESTROY:
            return (INT_PTR)FALSE;
    }
}

INT_PTR CALLBACK DlgProcLogin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            // 로그인 대화상자를 부모 대화상자 가운데에 출력
            HWND hParent = GetParent(hDlg);
            RECT rcParent, rcDlg, rc;
            GetWindowRect(hParent, &rcParent);
            GetWindowRect(hDlg, &rcDlg);
            CopyRect(&rc, &rcParent);           // rc = rcDlgMain;

            OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
            OffsetRect(&rc, -rc.left, -rc.top);
            OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);
            SetWindowPos(hDlg, HWND_TOP, rcParent.left + (rc.right / 2), rcParent.top + (rc.bottom / 2), 0, 0, SWP_NOSIZE);
            return (INT_PTR)TRUE;
        }
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    GetDlgItemText(hDlg, IDC_EDIT_LOGIN, loginID, 128);
                    EndDialog(hDlg, LOWORD(wParam));
                case IDCANCEL:
                    EndDialog(hDlg, LOWORD(wParam));
                    return (INT_PTR)TRUE;
            }
    }
    return (INT_PTR)FALSE;
}