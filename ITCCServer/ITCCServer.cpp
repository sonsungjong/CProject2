#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>
#include <Windows.h>
#pragma comment(lib, "WS2_32.lib")

#define PORT 23456
#define IP _T("127.0.0.1")

class UDPServerManager
{
    // ���� �ٸ� 5���� ���α׷����� UDP�� �޽����� �޴´�.
    // 5���� ���α׷��� ���� 11, 21, 31, 41, 51 �� �޽����� 1��~3�� ���̷� �ֱ������� �����´�.
    
    // 11�� ������ first�� 1�� ���
    // 21�� ������ second�� 2�� ���
    // 31���� ������ third�� 3�� ���
    // 41�� ������ forth�� 4�� ���
    // 51�� ������ fifth�� 5�� ���
private:
    StateManager& stateManager;

public:
    UDPServerManager(StateManager& sm) : stateManager(sm)
    {
        WSADATA data;
        (void)WSAStartup(0x0202, &data);
    }
    ~UDPServerManager()
    {
        WSACleanup();
    }

    void sendData()
    {

    }

    void recvData()
    {
        SOCKET listen_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        sockaddr_in addr_data = { AF_INET, htons(PORT), };
        InetPton(AF_INET, IP, &addr_data.sin_addr.s_addr);

        bind(listen_socket, (sockaddr*)&addr_data, sizeof(addr_data));

        sockaddr_in recv_socket = { 0 };
        ZeroMemory(&recv_socket, sizeof(recv_socket));
        int recv_len = sizeof(recv_socket);

        char buf[1024] = { 0 };

        while (true)
        {
            ZeroMemory(buf, 1024);

            int bytesIn = recvfrom(listen_socket, buf, 1024, 0, (sockaddr*)&recv_socket, &recv_len);
            if (bytesIn > 0) {
                buf[bytesIn] = NULL;
                if (strcmp(buf, "11") == 0) stateManager.first = '1';
                else if (strcmp(buf, "21") == 0) stateManager.second = '2';
                else if (strcmp(buf, "31") == 0) stateManager.third = '3';
                else if (strcmp(buf, "41") == 0) stateManager.forth = '4';
                else if (strcmp(buf, "51") == 0) stateManager.fifth = '5';
            }
        }

        closesocket(listen_socket);
    }
};

class StateManager 
{
public:
    char first;
    char second;
    char third;
    char forth;
    char fifth;
    TCPServerManager& tcpManager;

    StateManager(TCPServerManager& tm) : tcpManager(tm)
    {
        InitAllData();
    }

    void InitAllData()
    {
        first = 0;
        second = 0;
        third = 0;
        forth = 0;
        fifth = 0;
    }

    void startSending() {
        std::thread sendingThread([this]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                std::string state;
                state += first;
                state += second;
                state += third;
                state += forth;
                state += fifth;
                tcpManager.sendData(state);
                InitAllData();
            }
        });
        sendingThread.detach();
    }
};

class TCPServerManager
{
    // TCPServer
    // Ŭ���̾�Ʈ�� �񵿱�� �ۼ����Ѵ�

private:
    SOCKET serverSocket;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    LogViewer& logger;

public:
    TCPServerManager(LogViewer& log) : logger(log)
    {
        WSADATA data;
        (void)WSAStartup(MAKEWORD(2, 2), &data);

        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
        listen(serverSocket, SOMAXCONN);

        clientSocket = accept(serverSocket, 0, 0);
    }
    ~TCPServerManager() 
    {
        closesocket(clientSocket);
        WSACleanup();
    }

    void sendData(const std::string& data)
    {
        int sendResult = send(clientSocket, data.c_str(), data.size() + 1, 0);
        logger.logview(data);
    }

    void recvData()
    {
        
    }
};

class LogViewer
{
    // TCP�� �޽����� ���� �Ϳ� ���� printf�Ѵ�.
public:
    void logview(const std::string& msg) {
        printf("���� �޽����� �۽� - %s\n", msg.c_str());
    }
};

int main()
{
    LogViewer* logger = new LogViewer;
    TCPServerManager tcpServer(*logger);
    StateManager* stateManager = new StateManager(tcpServer);
    UDPServerManager udpServer(*stateManager);

    // UDP ���� �� ���� ������
    std::thread udpRecvThread([&udpServer]
    {
        udpServer.recvData();
    });

    // TCP ���� �� ���� ������
    std::thread tcpRecvThread([&tcpServer]
    {
        tcpServer.recvData();
    });

    udpRecvThread.join();
    tcpRecvThread.join();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    delete stateManager;
    delete logger;
}

