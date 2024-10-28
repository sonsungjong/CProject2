#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTS 50
#define BUFFER_SIZE 65507

class CTCPServer {
public:
    CTCPServer(const std::string& ip, uint16_t port, int workerThreadCount)
        : listenSocket(INVALID_SOCKET), iocp(nullptr), serverIP(ip), serverPort(port), workerThreadCount(workerThreadCount), isRunning(false) {}

    ~CTCPServer() {
        Stop();
        WSACleanup();
    }

    bool Initialize() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
        if (listenSocket == INVALID_SOCKET) {
            std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
            return false;
        }

        sockaddr_in serverAddr = {};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
        serverAddr.sin_port = htons(serverPort);

        if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
        if (!iocp) {
            std::cerr << "CreateIoCompletionPort failed: " << GetLastError() << std::endl;
            return false;
        }

        // Listen socket을 IOCP에 연결
        CreateIoCompletionPort((HANDLE)listenSocket, iocp, 0, 0);

        isRunning = true;

        // Create worker threads
        for (int i = 0; i < workerThreadCount; ++i) {
            workerThreads.emplace_back(&CTCPServer::WorkerThread, this);
        }

        return true;
    }

    void Run() {
        while (isRunning) {
            SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
                continue;
            }

            std::cout << "Client connected." << std::endl;

            // 클라이언트 소켓을 비동기 소켓으로 설정
            u_long mode = 1;
            ioctlsocket(clientSocket, FIONBIO, &mode);

            ClientContext* clientContext = new ClientContext;
            clientContext->socket = clientSocket;
            clientContext->wsabuf.buf = clientContext->buffer;
            clientContext->wsabuf.len = BUFFER_SIZE;
            ZeroMemory(&clientContext->overlapped, sizeof(WSAOVERLAPPED));

            CreateIoCompletionPort((HANDLE)clientSocket, iocp, (ULONG_PTR)clientContext, 0);

            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                clients.push_back(clientContext);
            }

            DWORD flags = 0;
            int result = WSARecv(clientSocket, &clientContext->wsabuf, 1, nullptr, &flags, &clientContext->overlapped, nullptr);
            if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
                std::cerr << "WSARecv failed: " << WSAGetLastError() << std::endl;
                RemoveClient(clientContext);
            }
        }
    }

    void Stop() {
        if (!isRunning) return;
        isRunning = false;

        // 모든 소켓 닫기
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            for (auto& client : clients) {
                closesocket(client->socket);
                delete client;
            }
            clients.clear();
        }

        // Listen 소켓 닫기
        if (listenSocket != INVALID_SOCKET) {
            closesocket(listenSocket);
            listenSocket = INVALID_SOCKET;
        }

        // IOCP에 종료 패킷 보내기
        for (size_t i = 0; i < workerThreads.size(); ++i) {
            PostQueuedCompletionStatus(iocp, 0, 0, nullptr);
        }

        // 워커 스레드 종료 대기
        for (auto& thread : workerThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        // IOCP 핸들 닫기
        if (iocp) {
            CloseHandle(iocp);
            iocp = nullptr;
        }
    }

    void sendData(const std::string& message) {
        std::lock_guard<std::mutex> lock(clientsMutex);
        for (auto& clientContext : clients) {
            if (clientContext && clientContext->socket != INVALID_SOCKET) {
                send(clientContext->socket, message.c_str(), static_cast<int>(message.size()), 0);
            }
        }
    }

private:
    struct ClientContext {
        SOCKET socket;
        char buffer[BUFFER_SIZE];
        WSAOVERLAPPED overlapped;
        WSABUF wsabuf;
    };

    SOCKET listenSocket;
    HANDLE iocp;
    std::string serverIP;
    uint16_t serverPort;
    int workerThreadCount;
    bool isRunning;
    std::vector<std::thread> workerThreads;
    std::vector<ClientContext*> clients;
    std::mutex clientsMutex;

    void RemoveClient(ClientContext* clientContext) {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.erase(std::remove(clients.begin(), clients.end(), clientContext), clients.end());
        closesocket(clientContext->socket);
        delete clientContext;
    }

    void WorkerThread() {
        DWORD bytesTransferred;
        ULONG_PTR completionKey;
        LPOVERLAPPED overlapped;

        while (isRunning) {
            BOOL success = GetQueuedCompletionStatus(iocp, &bytesTransferred, &completionKey, &overlapped, INFINITE);
            if (!success || bytesTransferred == 0) {
                ClientContext* clientContext = reinterpret_cast<ClientContext*>(completionKey);
                if (clientContext) {
                    std::cout << "Client disconnected." << std::endl;
                    RemoveClient(clientContext);
                }
                continue;
            }

            ClientContext* clientContext = reinterpret_cast<ClientContext*>(completionKey);

            std::cout << "Received data: " << std::string(clientContext->buffer, bytesTransferred) << std::endl;

            // Reissue WSARecv to keep reading data
            DWORD flags = 0;
            ZeroMemory(&clientContext->overlapped, sizeof(WSAOVERLAPPED));
            clientContext->wsabuf.buf = clientContext->buffer;
            clientContext->wsabuf.len = BUFFER_SIZE;
            int result = WSARecv(clientContext->socket, &clientContext->wsabuf, 1, nullptr, &flags, &clientContext->overlapped, nullptr);
            if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
                std::cerr << "WSARecv failed: " << WSAGetLastError() << std::endl;
                RemoveClient(clientContext);
            }
        }
    }
};

int main() {
    std::string ip = "127.0.0.1";
    uint16_t port = 8888;
    int threadCount = 4;

    CTCPServer server(ip, port, threadCount);
    if (!server.Initialize()) {
        return 1;
    }

    std::thread serverThread(&CTCPServer::Run, &server);

    std::string inputMessage;
    while (true) {
        std::cout << "\nEnter message to send to all clients:\n";
        std::getline(std::cin, inputMessage);
        if (inputMessage == "exit") {
            break;
        }
        server.sendData(inputMessage);
    }

    server.Stop();

    if (serverThread.joinable()) {
        serverThread.join();
    }

    return 0;
}
