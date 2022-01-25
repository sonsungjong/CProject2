// WinApi443AServer.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "WinApi443AServer.h"

#include <map>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS                          //framework.h 에 추가
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

#define WM_SERVER WM_USER+1
#define BUFSIZE 512

typedef enum {
    LOGIN = 1, LOGEXIST, LOGOUT, OTHERLOGIN, OTHERLOGOUT
} PACKET_TYPE;

typedef struct {
    PACKET_TYPE ptype;
    unsigned short len;
    char data[BUFSIZE];
}PACKET;

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
            WSAStartup(0x0202, &wsaData);                       // WinSock2.2 초기화
            server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);                     // IPv4, TCP stream, protocol

            SOCKADDR_IN addr;
            ZeroMemory(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = 10000;
            inet_pton(AF_INET, "loopback", &addr.sin_addr.s_addr);
            bind(server, (LPSOCKADDR)&addr, sizeof(addr));
            if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
                SetWindowText(hDlg, _T("listen error"));
                closesocket(server);                                                    // 소켓 종료1
                WSACleanup();                                                           // 소켓 종료2
            }
            SetWindowText(hDlg, _T("listening..."));
            WSAAsyncSelect(server, hDlg, WM_SERVER, FD_ACCEPT);
            return (INT_PTR)TRUE;
        }
        // wParam : socket, LOWORD(lParam) : network event
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
                case FD_READ:
                {
                    SOCKET client = (SOCKET)wParam;
                    int nR = recv(client, (char*)&packet, sizeof(PACKET), 0);
                    switch (packet.ptype)
                    {
                        case LOGIN:
                        {
                            memcpy(recvID, packet.data, packet.len);
                            if (clients.size() == 0)
                            {
                                clients.insert(std::pair<TSTRING, SOCKET>(TSTRING(recvID), client));
                                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                                _stprintf_s(strTemp, _countof(strTemp), _T("%d"), clients.size());
                                SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTemp);
                                send(client, (char*)&packet, sizeof(PACKET), 0);
                            }
                            else {
                                it = clients.find(TSTRING(recvID));
                                if (it == clients.end())            // not found
                                {
                                    // 로그인 브로드캐스팅
                                    for (it = clients.begin(); it != clients.end(); it++)
                                    {
                                        // 기존있는 패킷에 로그인 정보 보냄
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
                                    _stprintf_s(strTemp, _countof(strTemp), _T("%d"), clients.size());
                                    SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTemp);
                                    packet.ptype = LOGIN;
                                    packet.len = (unsigned short)(_tcslen(recvID) + 1) * sizeof(TCHAR);
                                    memcpy(packet.data, recvID, packet.len);
                                    send(client, (char*)&packet, sizeof(PACKET), 0);
                                }
                                else {              // found
                                    packet.ptype = LOGEXIST;
                                    send(client, (char*)&packet, sizeof(PACKET), 0);
                                }
                            }
                            break;
                        }
                        case LOGOUT:
                        {
                            // 로그아웃 브로드캐스팅
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

                            _stprintf_s(strTemp, _countof(strTemp), _T("%d"), clients.size());
                            SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTemp);

                            // ID를 찾아 리스트박스에서 삭제
                            unsigned int k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)recvID);
                            SendMessage(hList, LB_DELETESTRING, k, 0);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                case FD_CLOSE:
                {

                }
            }
    }
    return 0;
}