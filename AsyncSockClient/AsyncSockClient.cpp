/*
    TCP/IP ����� ���ؼ�
    ��ȭ���� �����ϰų� 
    �ش� ���α׷��� ��Ʈ�� ���� Windows ��ȭ�� ���ܸ� �߰��ؾ��Ѵ�.

    ������ -> �ý��� �� ���� -> Windows Defender ��ȭ�� -> ��� ���� -> ��� ������ ���Ե� Windows Defender ��ȭ��
    -> �ιٿ�� ��Ģ Ŭ�� -> �� ��Ģ Ŭ�� -> ���α׷� �Ǵ� ��Ʈ�� �����ϰ� ����
    -> �ƿ��ٿ�� ��Ģ ����(������ ����) -> �� ��Ģ Ŭ�� -> ���α׷� �Ǵ� ��Ʈ�� �����ϰ� ����
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

// ���̺귯�� ��ũ
#pragma comment(lib, "WS2_32.lib")

// ������ �޽����� ���� ���
void RecvMsg(SOCKET clientSocket)
{
    char buffer[1024];

    while (true)
    {
        // ���� ���� ��ٸ���
        memset(buffer, 0, 1024);
        int read = recv(clientSocket, buffer, sizeof(buffer)-1, 0);

        if (read > 0)
        {
            printf("��������>> %s \n", buffer);
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
        printf("������ ���� �޽���>>");
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

    // ���� �ּ� ����
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.S_un.S_addr = inet_addr(IP);

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
