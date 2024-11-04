#include "pch.h"
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream>
#include <string>
#include <queue>

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTS 5
#define BUFFER_SIZE 65507

typedef struct TCPMsg {
    unsigned char 송신자;
    unsigned char 수신자;
    int 메시지아이디;
    unsigned long long 바디크기;
    std::string strBodyMsg;
} ST_TCPMsg; // 14바이트

typedef struct ClientContext {
    SOCKET socket;
    WSAOVERLAPPED overlapped;
    WSABUF wsabuf;
    char buffer[BUFFER_SIZE];                // WSARecv 기본 제공버퍼
    std::vector<char> receiveBuffer;         // 누적 버퍼 저장공간
    size_t expectedBodySize = 0;             // 예상 바디 사이즈
} ST_ClientContext;

class CTCPServer {
public:
    CTCPServer(const std::string& ip, uint16_t port, int workerThreadCount)
        : m_listenSocket(INVALID_SOCKET), m_iocp(nullptr), m_serverIP(ip), m_serverPort(port), m_workerThreadCount(workerThreadCount), m_isRunning(false) {}

    ~CTCPServer() {
        Stop();
        WSACleanup();
    }

    bool Initialize() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup 실패: " << WSAGetLastError() << std::endl;
            return false;
        }

        m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
        if (m_listenSocket == INVALID_SOCKET) {
            std::cerr << "소켓 생성 실패: " << WSAGetLastError() << std::endl;
            return false;
        }

        sockaddr_in serverAddr = {};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(m_serverIP.c_str());
        serverAddr.sin_port = htons(m_serverPort);

        if (bind(m_listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "바인드 실패: " << WSAGetLastError() << std::endl;
            return false;
        }

        if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "리스닝 실패: " << WSAGetLastError() << std::endl;
            return false;
        }

        m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
        if (!m_iocp) {
            std::cerr << "CreateIoCompletionPort 실패: " << GetLastError() << std::endl;
            return false;
        }

        // 리슨 소켓을 IOCP에 연결
        CreateIoCompletionPort((HANDLE)m_listenSocket, m_iocp, 0, 0);

        m_isRunning = true;

        // 워커 스레드 생성
        for (int i = 0; i < m_workerThreadCount; ++i) {
            m_workerThreads.emplace_back(&CTCPServer::WorkerThread, this);
        }

        return true;
    }

    void Run() {
        while (m_isRunning) {
            SOCKET clientSocket = accept(m_listenSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept 실패: " << WSAGetLastError() << std::endl;
                continue;
            }

            std::cout << "클라이언트가 연결되었습니다." << std::endl;

            // 클라이언트 소켓을 비동기 소켓으로 설정
            u_long mode = 1;
            ioctlsocket(clientSocket, FIONBIO, &mode);

            ClientContext* clientContext = new ClientContext;
            clientContext->socket = clientSocket;
            clientContext->wsabuf.buf = clientContext->buffer;
            clientContext->wsabuf.len = BUFFER_SIZE;
            ZeroMemory(&clientContext->overlapped, sizeof(WSAOVERLAPPED));

            CreateIoCompletionPort((HANDLE)clientSocket, m_iocp, (ULONG_PTR)clientContext, 0);

            {
                std::lock_guard<std::mutex> lock(m_clientsMutex);
                m_clients.push_back(clientContext);
            }

            DWORD flags = 0;
            int result = WSARecv(clientSocket, &clientContext->wsabuf, 1, nullptr, &flags, &clientContext->overlapped, nullptr);
            if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
                std::cerr << "WSARecv 실패: " << WSAGetLastError() << std::endl;
                RemoveClient(clientContext);
            }
        }
    }

    void Stop() {
        if (!m_isRunning) return;
        m_isRunning = false;

        // 모든 소켓 닫기
        {
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            for (auto& client : m_clients) {
                closesocket(client->socket);
                delete client;
            }
            m_clients.clear();
        }

        // 리슨 소켓 닫기
        if (m_listenSocket != INVALID_SOCKET) {
            closesocket(m_listenSocket);
            m_listenSocket = INVALID_SOCKET;
        }

        // IOCP에 종료 패킷 보내기
        for (size_t i = 0; i < m_workerThreads.size(); ++i) {
            PostQueuedCompletionStatus(m_iocp, 0, 0, nullptr);
        }

        // 워커 스레드 종료 대기
        for (auto& thread : m_workerThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        // IOCP 핸들 닫기
        if (m_iocp) {
            CloseHandle(m_iocp);
            m_iocp = nullptr;
        }
    }

    void sendData(const std::string& message) {
        ST_TCPMsg msg = { 0, };
        msg.송신자 = 'B';
        msg.수신자 = 'F';
        msg.메시지아이디 = 1;
        msg.바디크기 = message.size();
        msg.strBodyMsg = message;

        std::string sendData = "";
        size_t nHeaderSize = sizeof(msg.송신자) + sizeof(msg.수신자) + sizeof(msg.메시지아이디) + sizeof(msg.바디크기);
        sendData.reserve(nHeaderSize + msg.바디크기);
        
        sendData.append(reinterpret_cast<char*>(&msg.송신자), sizeof(msg.송신자));
        sendData.append(reinterpret_cast<char*>(&msg.수신자), sizeof(msg.수신자));
        sendData.append(reinterpret_cast<char*>(&msg.메시지아이디), sizeof(msg.메시지아이디));
        sendData.append(reinterpret_cast<char*>(&msg.바디크기), sizeof(msg.바디크기));
        sendData.append(msg.strBodyMsg);

        {
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            for (auto& clientContext : m_clients) {
                if (clientContext && clientContext->socket != INVALID_SOCKET) {
                    const char* dataPtr = sendData.c_str();
                    int nTotalSent = 0;
                    int nDataSize = static_cast<int>(sendData.size());

                    while (nTotalSent < nDataSize) {
                        int nSent = send(clientContext->socket, dataPtr + nTotalSent, nDataSize - nTotalSent, 0);
                        if (nSent == SOCKET_ERROR) {
                            std::cerr << "전송 실패: " << WSAGetLastError() << "\n";
                            break;
                        }
                        nTotalSent += nSent;
                    }
                }
            }
        }
    }

private:
    SOCKET m_listenSocket;
    HANDLE m_iocp;
    std::string m_serverIP;
    unsigned short m_serverPort;
    int m_workerThreadCount;
    bool m_isRunning;
    std::vector<std::thread> m_workerThreads;
    std::vector<ClientContext*> m_clients;
    std::mutex m_clientsMutex;
    std::queue<ST_TCPMsg> m_queue;

    void RemoveClient(ClientContext* clientContext) {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        m_clients.erase(std::remove(m_clients.begin(), m_clients.end(), clientContext), m_clients.end());
        closesocket(clientContext->socket);
        delete clientContext;
    }

    void WorkerThread() {
        DWORD bytesTransferred;
        ULONG_PTR completionKey;
        LPOVERLAPPED overlapped;

        while (m_isRunning) {
            BOOL success = GetQueuedCompletionStatus(m_iocp, &bytesTransferred, &completionKey, &overlapped, INFINITE);
            if (!success || bytesTransferred == 0) {
                ClientContext* clientContext = reinterpret_cast<ClientContext*>(completionKey);
                if (clientContext) {
                    std::cout << "클라이언트가 연결을 종료했습니다." << std::endl;
                    RemoveClient(clientContext);
                }
                continue;
            }

            ClientContext* clientContext = reinterpret_cast<ClientContext*>(completionKey);

            // 수신된 데이터를 수신 버퍼에 추가
            clientContext->receiveBuffer.insert(clientContext->receiveBuffer.end(),
                clientContext->buffer,
                clientContext->buffer + bytesTransferred);

            // 완전한 메시지를 처리
            while (true) {
                // 헤더 크기(14바이트) 이상인지 확인
                if (clientContext->receiveBuffer.size() >= 14) {
                    // 헤더 파싱
                    ST_TCPMsg stMsg;
                    stMsg.송신자 = clientContext->receiveBuffer[0];
                    stMsg.수신자 = clientContext->receiveBuffer[1];

                    // 메시지아이디 추출 (4바이트)
                    memcpy(&stMsg.메시지아이디, &clientContext->receiveBuffer[2], sizeof(int));

                    // 바디크기 추출 (8바이트)
                    memcpy(&stMsg.바디크기, &clientContext->receiveBuffer[6], sizeof(size_t));

                    size_t totalMessageSize = 14 + static_cast<size_t>(stMsg.바디크기);

                    // 완전한 메시지가 있는지 확인
                    if (clientContext->receiveBuffer.size() >= totalMessageSize) {
                        // 바디 추출
                        stMsg.strBodyMsg.assign(clientContext->receiveBuffer.begin() + 14,
                            clientContext->receiveBuffer.begin() + totalMessageSize);

                        // 큐에 담기
                        m_queue.push(stMsg);

                        // 디버깅 //
                        std::string firstPart, lastPart;
                        if (stMsg.strBodyMsg.length() <= 200) {
                            // 메시지 전체 출력
                            firstPart = stMsg.strBodyMsg;
                            lastPart = "";
                        }
                        else {
                            firstPart = stMsg.strBodyMsg.substr(0, 100);
                            lastPart = stMsg.strBodyMsg.substr(stMsg.strBodyMsg.length() - 100, 100);
                        }

                        // 완전한 메시지 처리
                        std::cout << "수신된 메시지 - 송신자: " << static_cast<char>(stMsg.송신자)
                            << ", 수신자: " << static_cast<char>(stMsg.수신자)
                            << ", 메시지아이디: " << stMsg.메시지아이디
                            << ", 바디크기: " << stMsg.바디크기
                            << ", 메시지 내용: " << firstPart << "     " << lastPart
                            << "\n실제크기: " << std::to_string(stMsg.strBodyMsg.size())
                            << "\n";

                        // 디버깅 //

                        // 처리된 메시지를 버퍼에서 제거
                        clientContext->receiveBuffer.erase(clientContext->receiveBuffer.begin(),
                            clientContext->receiveBuffer.begin() + totalMessageSize);
                    }
                    else {
                        // 메시지의 나머지를 기다림
                        break;
                    }
                }
                else {
                    // 헤더의 나머지를 기다림
                    break;
                }
            }

            // WSARecv를 다시 호출하여 데이터를 계속 수신
            DWORD flags = 0;
            ZeroMemory(&clientContext->overlapped, sizeof(WSAOVERLAPPED));
            clientContext->wsabuf.buf = clientContext->buffer;
            clientContext->wsabuf.len = BUFFER_SIZE;
            int result = WSARecv(clientContext->socket, &clientContext->wsabuf, 1, nullptr, &flags, &clientContext->overlapped, nullptr);
            if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
                std::cerr << "WSARecv 실패: " << WSAGetLastError() << std::endl;
                RemoveClient(clientContext);
            }
        }
    }
};

int main() {
    std::string ip = "127.0.0.1";
    unsigned short port = 8888U;
    int threadCount = 4;

    CTCPServer server(ip, port, threadCount);
    if (!server.Initialize()) {
        return 1;
    }

    std::thread serverThread(&CTCPServer::Run, &server);

    std::string inputMessage;
    while (true) {
        std::cout << "\n모든 클라이언트에게 보낼 메시지를 입력하세요:\n";
        std::getline(std::cin, inputMessage);
        if (inputMessage == "exit") {
            break;
        }
        server.sendData(inputMessage);              // 헤더를 붙여서 바이트로 전송한다
    }

    server.Stop();

    if (serverThread.joinable()) {
        serverThread.join();
    }

    return 0;
}
