/*
    TCP/IP ����� ���ؼ�
    ��ȭ���� �����ϰų�
    �ش� ���α׷��� ��Ʈ�� ���� Windows ��ȭ�� ���ܸ� �߰��ؾ��Ѵ�.

    ������ -> �ý��� �� ���� -> Windows Defender ��ȭ�� -> ��� ���� -> ��� ������ ���Ե� Windows Defender ��ȭ��
    -> �ιٿ�� ��Ģ Ŭ�� -> �� ��Ģ Ŭ�� -> ���α׷� �Ǵ� ��Ʈ�� �����ϰ� ����
    -> �ƿ��ٿ�� ��Ģ ����(������ ����) -> �� ��Ģ Ŭ�� -> ���α׷� �Ǵ� ��Ʈ�� �����ϰ� ����
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define IP                         "192.168.196.189"
#define IP                         "192.168.196.252"
#define PORT                    18888

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
    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;

    while (true)
    {
        // ���� ���� ��ٸ���
        memset(buffer, 0, 1024);
        int read = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (read > 0)
        {
            printf("��������>> %s \n", buffer);
            // ���޹��� ���ڸ� �߶� Ȯ��
            if (buffer[4] == '-' && buffer[7] == '-' && buffer[10] == ' ' && buffer[13] == ':' && buffer[16] == ':') {
                sscanf_s(buffer, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
                // �ð�����
                SYSTEMTIME st;

                st.wYear = year; // �⵵�� �����մϴ�.
                st.wMonth = month; // ���� �����մϴ�.
                st.wDay = day; // ���� �����մϴ�.
                st.wHour = hour; // �ð��� �����մϴ�. (�ѱ�)
                st.wMinute = minute; // ���� �����մϴ�.
                st.wSecond = second; // �ʸ� �����մϴ�.

                SetSystemTime(&st);             // �����ڱ��� �ʿ� : �ý����� �ð��� �����Ѵ�
                printf("����Ϸ� - %d�� %d�� %d��, %d�� %d�� %d��\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
            }
            
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
    char client_msg[5] = "time";

    send(clientSocket, client_msg, strlen(client_msg), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}



int main()
{
    //HWND hWnd = GetConsoleWindow();
    //ShowWindow(hWnd, SW_HIDE); // SW_HIDE�� â�� ����� ����Դϴ�.

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
        //printf("���� ���� ����");
        return -1;
    }

    // �񵿱�� �޽��� �����۾� ����
    //std::future<void> future_recv = std::async(std::launch::async, RecvMsg, clientSocket);
    //std::future<void> future_send = std::async(std::launch::async, SendMsg, clientSocket);


    SendMsg(clientSocket);
    RecvMsg(clientSocket);

    // ���� �ݱ�
    closesocket(clientSocket);

    // ���̺귯�� ����
    WSACleanup();
}
