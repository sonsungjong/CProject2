#include <cstdio>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <WinSock2.h>

// ���̺귯�� ��ũ
#pragma comment(lib, "WS2_32.lib")

// �޽����� �����ϴ� �Լ�
void RecvMsg(SOCKET clientSocket)
{
    char buffer[1024];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        std::string strOut;

        // �޽��� ����
        int read = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (read > 0)
        {
            strOut = buffer;
            printf("Recv>> %s \n", strOut.c_str());

            // Ŭ���̾�Ʈ���� ����
            send(clientSocket, strOut.c_str(), strOut.size(), 0);
        }
        else if (read == 0)
        {
            printf("�޽��� ����");
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

    // ���̺귯�� ����
    if (WSAStartup(0x0202, &wsaData) != 0)
    {
        return -1;
    }

    // ���� ����
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.S_un.S_addr = INADDR_ANY;

    // ���� ����
    bind(serverSocket, (struct sockaddr*)(&serverAddress), sizeof(serverAddress));

    // ���� ���� ����
    listen(serverSocket, 5);

    // Ŭ���̾�Ʈ ����
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

    // �񵿱�� �޽��� �����Լ� ���
    std::future<void> future = std::async(std::launch::async, RecvMsg, clientSocket);

    while (true)
    {
        printf("Hello Server! \n");
        std::this_thread::sleep_for(std::chrono::seconds(1));

    }

    // ���� �ݱ�
    closesocket(serverSocket);
    // ���̺귯�� ����
    WSACleanup();
}

