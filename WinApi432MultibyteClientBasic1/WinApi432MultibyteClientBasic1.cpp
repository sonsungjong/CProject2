// WinApi432MultibyteClientBasic1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS                     // inet_addr
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

    // 1. ���� �ʱ�ȭ
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 2. ���� ����
    // IPv4, TCP stream, protocol
    client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = 10000;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");           // "loopback"

    // 3. ������ ����
    int nRet = connect(client, (LPSOCKADDR)&addr, sizeof(addr));
    if (nRet == SOCKET_ERROR) {
        closesocket(client);
        return 1;
    }
    cout << "The client is connected to the server." << endl;
    int nR, nS;
    char buf[BUFSIZE];
    do {
        cout << "[client]:";
        cin.getline(buf, BUFSIZE);
        // bye��� �Է��ϸ� ��������
        if (strcmp(buf, "bye") == 0) { break; }
        nS = send(client, buf, strlen(buf) + 1, 0);                         // 4. �۽�
        nR = recv(client, buf, BUFSIZE, 0);                                 // 5. ����
        cout << "[server]:" << buf << endl;
    } while (nR > 0);

    // 6. ���� �ݱ�
    closesocket(client);
    // 7. ���� ����
    WSACleanup();

    return 0;
}