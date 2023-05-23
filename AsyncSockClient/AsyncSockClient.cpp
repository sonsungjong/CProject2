#include <cstdio>
#include <string>
#include <thread>
#include <chrono>
#include <future>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>

// ���̺귯�� ��ũ
#pragma comment(lib, "WS2_32.lib")

// ������ �޽����� �����ϰ� ���� ���
void sendMsg(SOCKET clientSocket)
{
    char buffer[1024];
    const char* msg = "�ȳ��ϼ��� �ݰ����ϴ� Ŭ���̾�Ʈ�Դϴ�.";

    // �޽��� ����
    while (true)
    {
        send(clientSocket, msg, strlen(msg), 0);

        // ���� ���� ��ٸ���
        int read = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (read > 0)
        {
            std::string strOut = buffer;
            printf("��������>> %s \n", strOut.c_str());
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
    std::future<void> future = std::async(std::launch::async, sendMsg, clientSocket);

    while (true)
    {
        printf("Hello Client \n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // ���� �ݱ�
    closesocket(clientSocket);

    // ���̺귯�� ����
    WSACleanup();
}
