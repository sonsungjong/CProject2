// TicTacToeServer.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "TicTacToeServer.h"
#include <map>

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define IP "127.0.0.1"
#define PORT 44554

// Global Variables:
HINSTANCE hInst;                                // current instance
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DlgProc);

    return 0;
}

#define WM_SERVER    WM_USER+1
#define BUFSIZE     512

typedef enum {
    LOGIN=1, LOGEXIST, LOGOUT,
    OTHERLOGIN, OTHERLOGOUT,
    GAME_ASK, GAME_ACCEPT, GAME_REJECT,
    GAME_POINT, GAME_END
} PACKET_TYPE;

typedef struct {
    PACKET_TYPE ptype;
    unsigned short len;             // length(data)
    char data[BUFSIZE];
}PACKET;

#define BUFSIZE2    100
typedef struct {
    unsigned short lenFrom;
    TCHAR fromID[BUFSIZE2];

    unsigned short lenTo;
    TCHAR toID[BUFSIZE2];

    unsigned short ix;              // GAME_END 일때는 winner 저장
    unsigned short iy;
}GAME_DATA;

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hList;
    static SOCKET server;
    static std::map<TSTRING, SOCKET> clients;
    std::map<TSTRING, SOCKET>::iterator it;
    PACKET packet;
    TCHAR strTemp[256];
    TCHAR recvID[256];

    switch (message)
    {
        case WM_INITDIALOG:
        {
            hList = GetDlgItem(hDlg, IDC_LIST1);
            WSADATA wsaData;
            (void)WSAStartup(0x0202, &wsaData);               // WinSock2.2 초기화

            // IPv4, TCP stream, protocol
            server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            SOCKADDR_IN addr;
            ZeroMemory(&addr, sizeof(addr));
            addr.sin_family = AF_INET;              // IPv4
            addr.sin_port = PORT;
            int ret;
            ret = inet_pton(AF_INET, IP, &addr.sin_addr.s_addr);

            bind(server, (LPSOCKADDR)&addr, sizeof(addr));
            if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
                SetWindowText(hDlg, _T("listen error"));
                closesocket(server);
                WSACleanup();
                break;
            }
            SetWindowText(hDlg, _T("listening..."));
            ret = WSAAsyncSelect(server, hDlg, WM_SERVER, FD_ACCEPT);
            return (INT_PTR)TRUE;
        }

        // wParam: socket
        // LOWORD(lParam): network event, HIWORD(lParam): error code
        case WM_SERVER:
            switch (WSAGETSELECTEVENT(lParam))
            {
                case FD_ACCEPT:
                {
                    SOCKET client;
                    client = accept(server, nullptr, nullptr);
                    WSAAsyncSelect(client, hDlg, WM_SERVER, FD_READ | FD_CLOSE);
                    break;
                }
                case FD_READ:                   // 패킷 분석
                {
                    SOCKET client = (SOCKET)wParam;
                    int nR = recv(client, (char*)&packet, sizeof(PACKET), 0);
                    switch (packet.ptype)
                    {
                        case LOGIN:
                        {
                            memcpy(recvID, packet.data, packet.len);
                            if (clients.size() == 0)            // 첫 로그인
                            {
                                clients.insert(std::pair<TSTRING, SOCKET>(TSTRING(recvID), client));
                                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                                _stprintf_s(strTemp, sizeof(strTemp)/sizeof(strTemp[0]) ,_T("%d"), (int)clients.size());
                                SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTemp);
                                // reply to client
                                send(client, (char*)&packet, sizeof(PACKET), 0);
                            }
                            else {              // 검사하여 없을때만
                                it = clients.find(TSTRING(recvID));
                                if (it == clients.end())            // not found
                                {
                                    // 로그인 브로드 캐스팅
                                    for (it = clients.begin(); it != clients.end(); it++)
                                    {
                                        // 기존 있는 패킷에 로그인 정보 보냄
                                        packet.ptype = OTHERLOGIN;
                                        packet.len = (unsigned short)(_tcslen(recvID) + 1) * sizeof(TCHAR);
                                        memcpy(packet.data, recvID, packet.len);
                                        send(it->second, (char*)&packet, sizeof(PACKET), 0);
                                        
                                        // 로그인한 곳에 기존 정보 보냄
                                        _tcscpy_s(strTemp, it->first.c_str());
                                        packet.len = (unsigned short)(_tcslen(strTemp) + 1) * sizeof(TCHAR);
                                        memcpy(packet.data, strTemp, packet.len);
                                        send(client, (char*)&packet, sizeof(PACKET), 0);
                                    }

                                    // 로그인 피드백
                                    clients.insert(std::pair<TSTRING, SOCKET>(TSTRING(recvID), client));
                                    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                                    _stprintf_s(strTemp, sizeof(strTemp)/sizeof(strTemp[0]) ,_T("%d"), (int)clients.size());
                                    SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTemp);

                                    packet.ptype = LOGIN;
                                    packet.len = (unsigned short)(_tcslen(recvID) + 1) * sizeof(TCHAR);
                                    memcpy(packet.data, recvID, packet.len);
                                    send(client, (char*)&packet, sizeof(PACKET), 0);
                                }
                                else {          // found
                                    packet.ptype = LOGEXIST;
                                    send(client, (char*)&packet, sizeof(PACKET), 0);
                                }
                            }
                            break;
                        }
                        case LOGOUT:
                        {
                            // 로그아웃 브로드 캐스팅
                            for (it = clients.begin(); it != clients.end(); it++)
                            {
                                if (it->second == client) { 
                                    packet.ptype = LOGOUT; 
                                }
                                else {
                                    packet.ptype = OTHERLOGOUT; 
                                }
                                send(it->second, (char*)&packet, sizeof(PACKET), 0);
                            }
                            memcpy(recvID, packet.data, packet.len);
                            clients.erase(TSTRING(recvID));

                            _stprintf_s(strTemp, sizeof(strTemp) / sizeof(strTemp[0]), _T("%d"), (int)clients.size());
                            SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTemp);

                            // ID를 찾아 리스트박스에서 삭제
                            unsigned int k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)recvID);
                            SendMessage(hList, LB_DELETESTRING, k, 0);
                            break;
                        }
                        // 게임 데이터를 toID의 소켓에 전송
                        case GAME_ASK:
                        case GAME_ACCEPT:
                        case GAME_REJECT:
                        case GAME_POINT:
                        case GAME_END:
                        {
                            GAME_DATA gameData;
                            memcpy(&gameData, packet.data, packet.len);

                            // toID의 소켓을 찾아 보냄
                            it = clients.find(TSTRING(gameData.toID));
                            if (it != clients.end()) {
                                send(it->second, (char*)&packet, sizeof(PACKET), 0);
                            }
                            break;
                        }
                    }
                    break;
                }
                case FD_CLOSE:
                {
                    TCHAR strID[256];
                    SOCKET client = (SOCKET)wParam;
                    BOOL bFound = FALSE;

                    // 닫은 소켓의 로그인 ID 찾음
                    for (it = clients.begin(); it != clients.end(); ++it)
                    {
                        if (it->second == client)
                        {
                            _tcscpy_s(strID, it->first.c_str());
                            bFound = TRUE;
                            break;
                        }
                    }
                    // 로그인을 안하고 연결을 끊은 경우
                    if (!bFound) {
                        break; 
                    }
                    // 로그인을 하고 연결을 끊은 경우 브로드 캐스팅
                    for (it = clients.begin(); it != clients.end(); ++it)
                    {
                        if (it->second != client)
                        {
                            packet.ptype = OTHERLOGOUT;
                            packet.len = (unsigned short)(_tcslen(strID) + 1) * sizeof(TCHAR);
                            memcpy(packet.data, strID, packet.len);
                            send(it->second, (char*)&packet, sizeof(PACKET), 0);
                        }
                    }
                    clients.erase(TSTRING(strID));              // 소켓 삭제
                    unsigned int k;             // 리스트 박스에서 찾아 ID 삭제
                    k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)strID);
                    SendMessage(hList, LB_DELETESTRING, k, 0);
                    _stprintf_s(strID, sizeof(strID) / sizeof(strID[0]), _T("%d"), (int)clients.size());
                    SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strID);
                    closesocket(client);
                }
                break;
            }
            return (INT_PTR)TRUE;
        case WM_CLOSE:
            DestroyWindow(hDlg);
            return TRUE;
        case WM_DESTROY:
            closesocket(server);
            WSACleanup();
            return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}
