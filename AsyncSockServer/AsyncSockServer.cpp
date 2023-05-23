#include <cstdio>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <WinSock2.h>

// 라이브러리 링크
#pragma comment(lib, "WS2_32.lib")

// 메시지를 수신하는 함수
void RecvMsg(SOCKET clientSocket)
{
    char buffer[1024];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        std::string strOut;

        // 메시지 수신
        int read = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (read > 0)
        {
            strOut = buffer;
            printf("Recv>> %s \n", strOut.c_str());

            // 클라이언트에게 응답
            send(clientSocket, strOut.c_str(), strOut.size(), 0);
        }
        else if (read == 0)
        {
            printf("메시지 없음");
            break;
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

    // 라이브러리 실행
    if (WSAStartup(0x0202, &wsaData) != 0)
    {
        return -1;
    }

    // 소켓 생성
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.S_un.S_addr = INADDR_ANY;

    // 서버 설정
    bind(serverSocket, (struct sockaddr*)(&serverAddress), sizeof(serverAddress));

    // 서버 리슨 시작
    listen(serverSocket, 5);

    // 클라이언트 연결
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

    // 비동기로 메시지 수신함수 사용
    std::future<void> future = std::async(std::launch::async, RecvMsg, clientSocket);

    while (true)
    {
        printf("Hello Server! \n");
        std::this_thread::sleep_for(std::chrono::seconds(1));

    }

    // 소켓 닫기
    closesocket(serverSocket);
    // 라이브러리 종료
    WSACleanup();
}

