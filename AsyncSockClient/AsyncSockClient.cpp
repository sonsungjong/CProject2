#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <tchar.h>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <WinSock2.h>
#include <WS2tcpip.h>

// 라이브러리 링크
#pragma comment(lib, "WS2_32.lib")

/*
멀티바이트에서 유니코드로 변환 (char)
*/
void A2Wpchar(const char* const p_char, wchar_t* const p_wchar)
{
    int origin_len = strlen(p_char);
    int len = MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, NULL, NULL);
    MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, p_wchar, len);
}

/*
유니코드에서 멀티바이트로 변환 (char)
*/
void W2Apchar(const wchar_t* const p_wchar, char* const p_char)
{
    int len = WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, p_char, len, NULL, NULL);
}

/*
UTF-8에서 유니코드로 변환
*/
void Utf2Wpchar(const char* const p_char, wchar_t* const p_wchar)
{
    int origin_len = strlen(p_char);
    int len = MultiByteToWideChar(CP_UTF8, 0, p_char, origin_len, NULL, NULL);
    MultiByteToWideChar(CP_UTF8, 0, p_char, origin_len, p_wchar, len);
}

/*
유니코드에서 UTF-8로 변환
*/
void W2Utfpchar(const wchar_t* const p_wchar, char* const p_char)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, p_char, len, NULL, NULL);
}

// 서버의 메시지를 수신 대기
void RecvMsg(SOCKET clientSocket)
{
    char buffer[1024];

    while (true)
    {
        // 서버 응답 기다리기
        memset(buffer, 0, 1024);
        int read = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (read > 0)
        {
            printf("서버응답>> %s \n", buffer);
        }
        else if (read == 0)
        {
            printf("Server closed connection \n");
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        else
        {
            printf("Read Error \n");
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}

void SendMsg(SOCKET clientSocket)
{
    char client_msg[256] = { 0, };
    //TCHAR wmsg[256] = { 0, };

    while (true)
    {
        memset(client_msg, 0, 256);
        //wmemset(wmsg, 0, 256);
        printf("서버에 보낼 메시지>>");
        rewind(stdin);
        scanf_s("%[^\n]s", client_msg, 256);

        //A2Wpchar(client_msg, wmsg);
        //memset(client_msg, 0, 256);
        //W2Utfpchar(wmsg, client_msg);
    
        send(clientSocket, client_msg, strlen(client_msg), 0);
        //printf("서버로 보낸 메시지: %s", client_msg);
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
