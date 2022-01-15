// WinApi433UnicodeClientBasic1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <locale>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS                   // inet_addr
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define BUFSIZE 256
using namespace std;

int main()
{
    WSADATA wsaData;
    SOCKADDR_IN addr;
    SOCKET client;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //IPv4, TCP stream, protocol
    client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = 10000;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");               // "loopback"
    int nRet = connect(client, (LPSOCKADDR)&addr, sizeof(addr));
    if (nRet == SOCKET_ERROR) {
        closesocket(client);
        return 1;
    }
    wprintf(L"The client is connected to the server.\n");

    setlocale(LC_ALL, "");                  // 유니코드 한글출력
    int nR, nS;
    wchar_t buf[BUFSIZE];
    do {
        wcout << L"[client]:";
        wcin.getline(buf, BUFSIZE);
        if (wcscmp(buf, L"bye") == 0) {
            break;
        }
        nS = send(client, (char*)buf, (wcslen(buf) + 1) * sizeof(wchar_t), 0);
        wcout << L"nS=" << nS << endl;
        nR = recv(client, (char*)buf, BUFSIZE, 0);
        wcout << L"nR=" << nR << endl;

        wcout << L"[server]:" << buf << endl;
    } while (nR > 0);

    closesocket(client);            // 서버의 recv의 반환값이 0
    WSACleanup();

    return 0;
}
