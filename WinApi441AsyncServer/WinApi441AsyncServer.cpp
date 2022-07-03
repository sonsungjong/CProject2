// WinApi441AsyncServer.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WinApi441AsyncServer.h"
#include <string>
#include <map>

#define MAX_LOADSTRING 100
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

typedef std::basic_string<TCHAR> TSTRING;

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
#define BUFSIZE 256
typedef enum { LOGIN = 1, LOGEXIST, LOGOUT, MYMSG } PACKET_TYPE;
typedef struct {
    PACKET_TYPE ptype;
    unsigned short len;     // length(data)
    char data[BUFSIZE];
} PACKET;

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hList;
    static SOCKET server;
    static std::map<TSTRING, SOCKET> clients;
    std::map<TSTRING, SOCKET>::iterator it;
    PACKET packet;
    TCHAR recvID[40];
    TCHAR strTemp[40];

    switch (message)
    {
        case WM_INITDIALOG:
        {
            hList = GetDlgItem(hDlg, IDC_LIST1);
            WSADATA wsaData;
            WSAStartup(0x0202, &wsaData);
            
            // IPv4, TCP stream, protocol
            server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            SOCKADDR_IN addr;
            ZeroMemory(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = 10005;
            inet_pton(AF_INET, "loopback", &addr.sin_addr.s_addr);
            bind(server, (LPSOCKADDR)&addr, sizeof(addr));
            if (listen(server, SOMAXCONN) == SOCKET_ERROR) 
            {
                SetWindowText(hDlg, _T("listen error"));
                closesocket(server);
                WSACleanup();
                break;
            }
            SetWindowText(hDlg, _T("listening..."));
            WSAAsyncSelect(server, hDlg, WM_SERVER, FD_ACCEPT);
            return (INT_PTR)TRUE;
        }

        // wParam:socket
        //LOWORD(lParam): network event, HIWORD(lParam): error code
        case WM_SERVER:
            switch (WSAGETSELECTEVENT(lParam))              // LOWORD(lParam)
            {
            case FD_ACCEPT:
                {
                    // FD_READ에서 ID 확인 후 clients에 등록
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
                            }
                            else {
                                it = clients.find(TSTRING(recvID));
                                if (it == clients.end())
                                {
                                    clients.insert(std::pair<TSTRING, SOCKET>(TSTRING(recvID), client));
                                    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                                    _stprintf_s(strTemp, _countof(strTemp), _T("%d"), clients.size());
                                    SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTemp);
                                }
                                else {                 // found
                                    packet.ptype = LOGEXIST;
                                }
                            }
                            //reply to client
                            send(client, (char*)&packet, sizeof(PACKET), 0);
                            break;
                        }
                        case LOGOUT:
                        {
                            // reply to client
                            send(client, (char*)&packet, sizeof(PACKET), 0);
                            // clients에서 삭제
                            memcpy(recvID, packet.data, packet.len);
                            clients.erase(TSTRING(recvID));
                            _stprintf_s(strTemp, _countof(strTemp), _T("%d"), clients.size());
                            SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTemp);

                            // ID를 찾아 리스트 박스에서 삭제
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
                    TCHAR strID[256];
                    SOCKET client = (SOCKET)wParam;

                    // 닫은 소켓은 찾아, 삭제
                    for (it = clients.begin(); it != clients.end();)
                    {
                        if (it->second == client)
                        {
                            memcpy(strID, it->first.c_str(), (it->first.length() + 1) * sizeof(TCHAR));
                            // _tcscpy_s(strID, it->first.c_str());
                            clients.erase(it++);
                        }else { it++; }
                    }
                    unsigned int k;             // 리스트 박스에서 찾아 ID 삭제
                    k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)strID);
                    _stprintf_s(strTemp, _countof(strTemp), _T("%d"), clients.size());
                    SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTemp);
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