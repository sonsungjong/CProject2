// ConsoleTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <cstdio>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    addrinfo hints;
    addrinfo* result;

    WSADATA wsaData;
    // 1. 소켓 초기화
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;                              // IPv4
    hints.ai_socktype = SOCK_STREAM;                // TCP stream
    hints.ai_protocol = IPPROTO_TCP;                    // TCP protocol

    int nRet;
    // 2. IP주소 정보 가져오기
    nRet = getaddrinfo(nullptr, "512", &hints, &result);
    //nRet = getaddrinfo("loopback", "512", &hints, &result);
    //nRet = getaddrinfo("127.0.0.1", "512", &hints, &result);
    //nRet = getaddrinfo("www.google.com", "80", &hints, &result);
    if (nRet != 0) {
        printf("getaddrinfo failed with error: %d\n", nRet);
        WSACleanup();
        return 1;
    }
    printf("result->ai_family = %d\n", result->ai_family);
    printf("result->ai_socktype = %d\n", result->ai_socktype);
    printf("result->ai_protocol = %d\n", result->ai_protocol);
    printf("result->ai_addr->sa_family= %d\n", result->ai_addr->sa_family);

    //for (int i = 0; i < 14; i++) {
    //    printf("result->ai_addr->sa_data[%d] = %d\n", i, (BYTE)result->ai_addr->sa_data[i]);
    //}
    unsigned short port = MAKEWORD(result->ai_addr->sa_data[0], result->ai_addr->sa_data[1]);
    printf("port = %hu\n", ntohs(port));                // 리틀엔디안

    // SOCKADDR to SOCKADDR_IN 주소 변환
    SOCKADDR_IN* addrIPv4 = (SOCKADDR_IN*)(result->ai_addr);
    printf("addrIPv4->sin_family= %d\n", addrIPv4->sin_family);
    printf("addrIPv4->sin_port= %hu\n", ntohs(addrIPv4->sin_port));

    // IN_ADDR sin_addr
    printf("addrIPv4->sin_addr = %d\n", addrIPv4->sin_addr.S_un.S_un_b.s_b1);
    printf("addrIPv4->sin_addr = %d\n", addrIPv4->sin_addr.S_un.S_un_b.s_b2);
    printf("addrIPv4->sin_addr = %d\n", addrIPv4->sin_addr.S_un.S_un_b.s_b3);
    printf("addrIPv4->sin_addr = %d\n", addrIPv4->sin_addr.S_un.S_un_b.s_b4);
    // 3. 메모리해제
    freeaddrinfo(result);
    // 4. 소켓사용 종료
    WSACleanup();
}
