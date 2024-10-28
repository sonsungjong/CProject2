#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 65507

void ReceiveData(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    int result;
    while (true) {
        ZeroMemory(buffer, BUFFER_SIZE);
        result = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (result > 0) {
            std::cout << "Server: " << std::string(buffer, result) << std::endl;
        }
        else if (result == 0) {
            std::cout << "Connection closed by server." << std::endl;
            break;
        }
        else {
            std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
            break;
        }
    }
}

int main() {
    std::string serverIP = "127.0.0.1";
    uint16_t serverPort = 8888;

    WSADATA wsaData;
    SOCKET clientSocket = INVALID_SOCKET;
    sockaddr_in serverAddr;

    // 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddr.sin_port = htons(serverPort);

    // 서버에 연결
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    // 서버로부터 데이터를 수신하는 스레드 시작
    std::thread recvThread(ReceiveData, clientSocket);

    // 메인 스레드는 사용자 입력을 받아 서버로 전송
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == "exit") {
            break;
        }
        int sendResult = send(clientSocket, message.c_str(), message.size(), 0);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            break;
        }
    }

    // 종료 처리
    closesocket(clientSocket);
    WSACleanup();

    // 수신 스레드 종료 대기
    if (recvThread.joinable()) {
        recvThread.join();
    }

    return 0;
}
