#include <cstdio>
#include <string>
#include <thread>
#include <chrono>
#include <future>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>

// 라이브러리 링크
#pragma comment(lib, "WS2_32.lib")

// 서버에 메시지를 전송하고 수신 대기
void sendMsg(SOCKET clientSocket)
{
    char buffer[1024];
    const char* msg = "안녕하세요 반갑습니다 클라이언트입니다.";

    // 메시지 전송
    while (true)
    {
        send(clientSocket, msg, strlen(msg), 0);

        // 서버 응답 기다리기
        int read = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (read > 0)
        {
            std::string strOut = buffer;
            printf("서버응답>> %s \n", strOut.c_str());
        }
        else if (read == 0)
        {
            printf("Server closed connection \n");
        }
        else
        {
            printf("Read Error \n");
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    // 서버와 연결
    if (connect(clientSocket, (struct sockaddr*)(&serverAddress), sizeof(serverAddress)) < 0)
    {
        printf("서버 연결 실패");
        return -1;
    }

    // 비동기로 메시지 전송작업 시작
    std::future<void> future = std::async(std::launch::async, sendMsg, clientSocket);

    while (true)
    {
        printf("Hello Client \n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 소켓 닫기
    closesocket(clientSocket);

    // 라이브러리 종료
    WSACleanup();
}
