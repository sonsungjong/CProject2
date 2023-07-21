/*
    TCP/IP 통신을 위해선
    방화벽을 해제하거나 
    해당 프로그램과 포트에 대해 Windows 방화벽 예외를 추가해야한다.

    제어판 -> 시스템 및 보안 -> Windows Defender 방화벽 -> 고급 설정 -> 고급 보안이 포함된 Windows Defender 방화벽
    -> 인바운드 규칙 클릭 -> 새 규칙 클릭 -> 프로그램 또는 포트를 선택하고 다음
    -> 아웃바운드 규칙 생성(나가는 연결) -> 새 규칙 클릭 -> 프로그램 또는 포트를 선택하고 다음
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define IP                         "172.16.1.226" 
#define PORT                    8000

#include <cstdio>
#include <tchar.h>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <WinSock2.h>

// 라이브러리 링크
#pragma comment(lib, "WS2_32.lib")

// 서버의 메시지를 수신 대기
void RecvMsg(SOCKET clientSocket)
{
    char buffer[1024];

    while (true)
    {
        // 서버 응답 기다리기
        memset(buffer, 0, 1024);
        int read = recv(clientSocket, buffer, sizeof(buffer)-1, 0);

        if (read > 0)
        {
            printf("서버응답>> %s \n", buffer);
        }
        else if (read <= 0)
        {
            printf("Server closed connection \n");
            return;
        }
        else
        {
            printf("Read Error \n");
            return;
        }
    }
}

void SendMsg(SOCKET clientSocket)
{
    char client_msg[256] = { 0, };
    // {\r\n    \"UI\":\"req\"\r\n}

    while (true)
    {
        memset(client_msg, 0, 256);
        printf("서버에 보낼 메시지>>");
        rewind(stdin);
        scanf_s("%[^\n]s", client_msg, 256);
    
        send(clientSocket, client_msg, strlen(client_msg), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}



int main()
{
    WSADATA wsaData;

    if (WSAStartup(0x0202, &wsaData) != 0)
    {
        return -1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 서버 주소 설정
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.S_un.S_addr = inet_addr(IP);

    // 서버와 연결
    if (connect(clientSocket, (struct sockaddr*)(&serverAddress), sizeof(serverAddress)) < 0)
    {
        printf("서버 연결 실패");
        return -1;
    }

    // 비동기로 메시지 전송작업 시작
    std::future<void> future_recv = std::async(std::launch::async, RecvMsg, clientSocket);
    std::future<void> future_send = std::async(std::launch::async, SendMsg, clientSocket);

    while (true)
    {
        printf("\n =======Client UI======= \n");
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    // 소켓 닫기
    closesocket(clientSocket);

    // 라이브러리 종료
    WSACleanup();
}
