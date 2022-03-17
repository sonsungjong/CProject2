// WinApi432MultibyteServerBasic1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS             // inet_addr
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>                       // inet_pton
#pragma comment(lib, "Ws2_32.lib")

using namespace std;
#define BUFSIZE 256

// 1���� Ŭ���̾�Ʈ�� ���� �� �ִ� ��Ƽ����Ʈ ���� (ASCII �ۼ���)
int main()
{
    WSADATA wsaData;
    SOCKADDR_IN addr;
    SOCKET server;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // IPv4, TCP stream, protocol
    server = socket(AF_INET, SOCK_STREAM , IPPROTO_TCP);
    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = 10000;
    inet_pton(AF_INET, "loopback", &addr.sin_addr.s_addr);
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(server, (LPSOCKADDR)&addr, sizeof(addr));

    // ���� ��� listen
    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen error: %ld\n", WSAGetLastError());
        closesocket(server);
        WSACleanup();
        return 1;
    }
    cout << " The server is listening..." << endl;

    SOCKET client;
    // ���� ���� accept
    client = accept(server, nullptr, nullptr);
    if (client == INVALID_SOCKET) {
        closesocket(server);
        WSACleanup();
        return 1;
    }
    cout << "A client is accepted." << endl;
    closesocket(server);                // 1�� Ŭ���̾�Ʈ�� �ް�, ���� ����

    int nR, nS;
    char buf[BUFSIZE];
    do {
        nR = recv(client, buf, BUFSIZE, 0);
        nS = send(client, buf, nR, 0);
        if (nS == SOCKET_ERROR)
            break;
    } while (nR > 0);

    // shutdown(client, SD_SEND);
    closesocket(client);
    WSACleanup();
    return 0;
}
