#include "pch.h"
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 262100

// TCPMsg 구조체 정의
typedef struct TCPMsg {
    unsigned char 송신자;
    unsigned char 수신자;
    int 메시지아이디;
    unsigned long long 바디크기;
    std::string strBodyMsg;
} ST_TCPMsg; // 총 14바이트의 헤더

void ReceiveData(SOCKET clientSocket) {
    char* buffer = new char[BUFFER_SIZE]{ 0 };
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
    delete[] buffer;
}

int main() {
    std::string serverIP = "127.0.0.1";
    uint16_t serverPort = 8888;

    WSADATA wsaData;
    SOCKET clientSocket = INVALID_SOCKET;
    sockaddr_in serverAddr;

    // 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup 실패: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "소켓 생성 실패: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddr.sin_port = htons(serverPort);

    // 서버에 연결
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "연결 실패: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "서버에 연결되었습니다." << std::endl;

    // 서버로부터 데이터를 수신하는 스레드 시작
    std::thread recvThread(ReceiveData, clientSocket);

    // 매 1초마다 21만 바이트의 메시지를 서버로 전송
    while (true) {
        // 메시지 생성
        ST_TCPMsg msg;
        msg.송신자 = 'F';
        msg.수신자 = 'B';
        msg.메시지아이디 = 19;
        msg.바디크기 = 210000; // 바이트크기
        msg.strBodyMsg = std::string(210000, 'A'); // 'A'로 채워진 문자열
        msg.strBodyMsg.replace(msg.strBodyMsg.size() - 10, 10, 10, 'B');        // 마지막 10글자만 'B'로 바꾼다


        // 메시지 직렬화
        std::vector<char> sendBuffer;
        sendBuffer.reserve(14 + msg.strBodyMsg.size());

        // 송신자 추가
        sendBuffer.push_back(msg.송신자);
        // 수신자 추가
        sendBuffer.push_back(msg.수신자);

        // 메시지아이디 추가
        int 메시지아이디 = msg.메시지아이디;
        sendBuffer.insert(sendBuffer.end(),
            reinterpret_cast<char*>(&메시지아이디),
            reinterpret_cast<char*>(&메시지아이디) + sizeof(int));

        // 바디크기 추가
        unsigned long long 바디크기 = msg.바디크기;
        sendBuffer.insert(sendBuffer.end(),
            reinterpret_cast<char*>(&바디크기),
            reinterpret_cast<char*>(&바디크기) + sizeof(unsigned long long));

        // 메시지 바디 추가
        sendBuffer.insert(sendBuffer.end(),
            msg.strBodyMsg.begin(),
            msg.strBodyMsg.end());

        // 서버로 메시지 전송
        int totalSent = 0;
        int toSend = static_cast<int>(sendBuffer.size());
        while (totalSent < toSend) {
            int sent = send(clientSocket, sendBuffer.data() + totalSent, toSend - totalSent, 0);
            if (sent == SOCKET_ERROR) {
                std::cerr << "전송 실패: " << WSAGetLastError() << std::endl;
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
            totalSent += sent;
        }

        std::cout << "서버로 메시지를 전송했습니다." << std::endl;

        // 1초 대기
        Sleep(1000);
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
