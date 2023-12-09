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
    // 서로 다른 5개의 프로그램에서 UDP로 메시지를 받는다.
    // 5개의 프로그램은 각각 11, 21, 31, 41, 51 로 메시지를 1초~3초 사이로 주기적으로 보내온다.
    
    // 11로 들어오면 first에 1을 담고
    // 21로 들어오면 second에 2를 담고
    // 31으로 들어오면 third에 3을 담고
    // 41로 들어오면 forth에 4을 담고
    // 51로 들어오면 fifth에 5를 담고
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
    // 클라이언트와 비동기로 송수신한다

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
    // TCP로 메시지를 보낸 것에 대해 printf한다.
public:
    void logview(const std::string& msg) {
        printf("다음 메시지를 송신 - %s\n", msg.c_str());
    }
};

int main()
{
    LogViewer* logger = new LogViewer;
    TCPServerManager tcpServer(*logger);
    StateManager* stateManager = new StateManager(tcpServer);
    UDPServerManager udpServer(*stateManager);

    // UDP 수신 및 저장 스레드
    std::thread udpRecvThread([&udpServer]
    {
        udpServer.recvData();
    });

    // TCP 수신 및 저장 스레드
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

