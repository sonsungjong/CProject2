#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <tchar.h>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <WinSock2.h>
#include <WS2tcpip.h>

// ���̺귯�� ��ũ
#pragma comment(lib, "WS2_32.lib")

/*
��Ƽ����Ʈ���� �����ڵ�� ��ȯ (char)
*/
void A2Wpchar(const char* const p_char, wchar_t* const p_wchar)
{
    int origin_len = strlen(p_char);
    int len = MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, NULL, NULL);
    MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, p_wchar, len);
}

/*
�����ڵ忡�� ��Ƽ����Ʈ�� ��ȯ (char)
*/
void W2Apchar(const wchar_t* const p_wchar, char* const p_char)
{
    int len = WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, p_char, len, NULL, NULL);
}

/*
UTF-8���� �����ڵ�� ��ȯ
*/
void Utf2Wpchar(const char* const p_char, wchar_t* const p_wchar)
{
    int origin_len = strlen(p_char);
    int len = MultiByteToWideChar(CP_UTF8, 0, p_char, origin_len, NULL, NULL);
    MultiByteToWideChar(CP_UTF8, 0, p_char, origin_len, p_wchar, len);
}

/*
�����ڵ忡�� UTF-8�� ��ȯ
*/
void W2Utfpchar(const wchar_t* const p_wchar, char* const p_char)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, p_char, len, NULL, NULL);
}

// ������ �޽����� ���� ���
void RecvMsg(SOCKET clientSocket)
{
    char buffer[1024];

    while (true)
    {
        // ���� ���� ��ٸ���
        memset(buffer, 0, 1024);
        int read = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (read > 0)
        {
            printf("��������>> %s \n", buffer);
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
        printf("������ ���� �޽���>>");
        rewind(stdin);
        scanf_s("%[^\n]s", client_msg, 256);

        //A2Wpchar(client_msg, wmsg);
        //memset(client_msg, 0, 256);
        //W2Utfpchar(wmsg, client_msg);
    
        send(clientSocket, client_msg, strlen(client_msg), 0);
        //printf("������ ���� �޽���: %s", client_msg);
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

    // ���� �ּ� ����
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    // ������ ����
    if (connect(clientSocket, (struct sockaddr*)(&serverAddress), sizeof(serverAddress)) < 0)
    {
        printf("���� ���� ����");
        return -1;
    }

    // �񵿱�� �޽��� �����۾� ����
    std::future<void> future_recv = std::async(std::launch::async, RecvMsg, clientSocket);
    std::future<void> future_send = std::async(std::launch::async, SendMsg, clientSocket);

    while (true)
    {
        printf("\n =======Client UI======= \n");
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    // ���� �ݱ�
    closesocket(clientSocket);

    // ���̺귯�� ����
    WSACleanup();
}
