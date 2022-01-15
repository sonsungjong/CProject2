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

    // 1. 소켓 초기화
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 2. 소켓 생성
    // IPv4, TCP stream, protocol
    client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = 10000;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");           // "loopback"

    // 3. 서버에 접속
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
        // bye라고 입력하면 접속해제
        if (strcmp(buf, "bye") == 0) { break; }
        nS = send(client, buf, strlen(buf) + 1, 0);                         // 4. 송신
        nR = recv(client, buf, BUFSIZE, 0);                                 // 5. 수신
        cout << "[server]:" << buf << endl;
    } while (nR > 0);

    // 6. 소켓 닫기
    closesocket(client);
    // 7. 소켓 종료
    WSACleanup();

    return 0;
}