#include "pch.h"
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 262100
#define HEADER_SIZE 14

// TCPMsg 구조체 정의
#pragma pack(push, 1)
typedef struct TCPMsg {
    unsigned char 송신자;
    unsigned char 수신자;
    int 메시지아이디;
    unsigned long long 바디크기;
    std::string strBodyMsg;
} ST_TCPMsg; // 총 14바이트의 헤더
#pragma pack(pop)

class CTCPClient
{
public:
    CTCPClient(const std::string& _strIP, unsigned short _usPort);
    virtual ~CTCPClient();

    bool initialize();
    void sendMsg(const ST_TCPMsg& msg);
    void sendMsg(const std::string& msg);
    void startReceiving();
    void stopReceiving();

    void recvMsg();
    void cleanup();

private:
    std::string m_serverIP;
    unsigned short m_serverPort;
    unsigned long long m_clientSocket;
    bool m_isConnected;
    std::thread m_recvThread;

    
};

//void ReceiveData(SOCKET clientSocket) {
//    char* buffer = new char[BUFFER_SIZE]{ 0 };
//    int result;
//    while (true) {
//        ZeroMemory(buffer, BUFFER_SIZE);
//        result = recv(clientSocket, buffer, BUFFER_SIZE, 0);
//        if (result > 0) {
//            std::cout << "Server: " << std::string(buffer, result) << std::endl;
//        }
//        else if (result == 0) {
//            std::cout << "Connection closed by server." << std::endl;
//            break;
//        }
//        else {
//            std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
//            break;
//        }
//    }
//    delete[] buffer;
//}
//
//int main() {
//    std::string serverIP = "127.0.0.1";
//    uint16_t serverPort = 8888;
//
//    WSADATA wsaData;
//    SOCKET clientSocket = INVALID_SOCKET;
//    sockaddr_in serverAddr;
//
//    // 윈속 초기화
//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//        std::cerr << "WSAStartup 실패: " << WSAGetLastError() << std::endl;
//        return 1;
//    }
//
//    // 소켓 생성
//    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (clientSocket == INVALID_SOCKET) {
//        std::cerr << "소켓 생성 실패: " << WSAGetLastError() << std::endl;
//        WSACleanup();
//        return 1;
//    }
//
//    // 서버 주소 설정
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
//    serverAddr.sin_port = htons(serverPort);
//
//    // 서버에 연결
//    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
//        std::cerr << "연결 실패: " << WSAGetLastError() << std::endl;
//        closesocket(clientSocket);
//        WSACleanup();
//        return 1;
//    }
//
//    std::cout << "서버에 연결되었습니다." << std::endl;
//
//    // 서버로부터 데이터를 수신하는 스레드 시작
//    std::thread recvThread(ReceiveData, clientSocket);
//
//    // 매 1초마다 21만 바이트의 메시지를 서버로 전송
//    while (true) {
//        // 메시지 생성
//        ST_TCPMsg msg;
//        msg.송신자 = 'F';
//        msg.수신자 = 'B';
//        msg.메시지아이디 = 19;
//        msg.바디크기 = 210000; // 바이트크기
//        msg.strBodyMsg = std::string(210000, 'A'); // 'A'로 채워진 문자열
//        msg.strBodyMsg.replace(msg.strBodyMsg.size() - 10, 10, 10, 'B');        // 마지막 10글자만 'B'로 바꾼다
//
//
//        // 메시지 직렬화
//        std::vector<char> sendBuffer;
//        sendBuffer.reserve(14 + msg.strBodyMsg.size());
//
//        // 송신자 추가
//        sendBuffer.push_back(msg.송신자);
//        // 수신자 추가
//        sendBuffer.push_back(msg.수신자);
//
//        // 메시지아이디 추가
//        int 메시지아이디 = msg.메시지아이디;
//        sendBuffer.insert(sendBuffer.end(),
//            reinterpret_cast<char*>(&메시지아이디),
//            reinterpret_cast<char*>(&메시지아이디) + sizeof(int));
//
//        // 바디크기 추가
//        unsigned long long 바디크기 = msg.바디크기;
//        sendBuffer.insert(sendBuffer.end(),
//            reinterpret_cast<char*>(&바디크기),
//            reinterpret_cast<char*>(&바디크기) + sizeof(unsigned long long));
//
//        // 메시지 바디 추가
//        sendBuffer.insert(sendBuffer.end(),
//            msg.strBodyMsg.begin(),
//            msg.strBodyMsg.end());
//
//        // 서버로 메시지 전송
//        int totalSent = 0;
//        int toSend = static_cast<int>(sendBuffer.size());
//        while (totalSent < toSend) {
//            int sent = send(clientSocket, sendBuffer.data() + totalSent, toSend - totalSent, 0);
//            if (sent == SOCKET_ERROR) {
//                std::cerr << "전송 실패: " << WSAGetLastError() << std::endl;
//                closesocket(clientSocket);
//                WSACleanup();
//                return 1;
//            }
//            totalSent += sent;
//        }
//
//        std::cout << "서버로 메시지를 전송했습니다." << std::endl;
//
//        // 1초 대기
//        Sleep(1000);
//    }
//
//    // 종료 처리
//    closesocket(clientSocket);
//    WSACleanup();
//
//    // 수신 스레드 종료 대기
//    if (recvThread.joinable()) {
//        recvThread.join();
//    }
//
//    return 0;
//}

CTCPClient::CTCPClient(const std::string& _strIP, unsigned short _usPort)
    : m_serverIP(_strIP), m_serverPort(_usPort), m_clientSocket(INVALID_SOCKET), m_isConnected(false)
{

}

CTCPClient::~CTCPClient()
{
    cleanup();
}

bool CTCPClient::initialize()
{
    WSADATA wsaData;
    // 1
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup 실패: " << WSAGetLastError() << "\n";
        return false;
    }

    // 2
    m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_clientSocket == INVALID_SOCKET) {
        std::cerr << "소켓 생성 실패: " << WSAGetLastError() << "\n";
        WSACleanup();
        return false;
    }

    // 3
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(m_serverIP.c_str());
    serverAddr.sin_port = htons(m_serverPort);

    int nRetryConnCount = 0;
    const int nMaxRetry = 20;               // 20번까지 시도
    const int retryDelay = 3000;            // 실패시 3초 대기
    m_isConnected = false;

    while (nRetryConnCount < nMaxRetry) {
        if (connect(m_clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "연결 실패: " << WSAGetLastError() << "\n";
            nRetryConnCount++;          // 재시도횟수 증가
            std::cerr << "재시도 중... (" << nRetryConnCount << "/" << nMaxRetry << ")\n";
            Sleep(retryDelay);
        }
        else {
            m_isConnected = true;
            printf("서버에 연결되었습니다\n");
            break;
        }
    }

    return m_isConnected;
}

void CTCPClient::sendMsg(const std::string& msg)
{
    if (m_isConnected) {
        ST_TCPMsg stMsg = { 0, };
        stMsg.송신자 = 'F';
        stMsg.수신자 = 'B';
        stMsg.메시지아이디 = 2;
        stMsg.바디크기 = msg.size();
        stMsg.strBodyMsg = msg;

        sendMsg(stMsg);
    }
}

void CTCPClient::sendMsg(const ST_TCPMsg& msg)
{
    if (m_isConnected) {
        std::vector<char> sendBuffer;
        sendBuffer.reserve(14 + msg.strBodyMsg.size());

        // 메시지 구성
        sendBuffer.push_back(msg.송신자);
        sendBuffer.push_back(msg.수신자);

        int nMsgID = msg.메시지아이디;
        sendBuffer.insert(sendBuffer.end(), reinterpret_cast<char*>(&nMsgID), reinterpret_cast<char*>(&nMsgID) + sizeof(int));

        unsigned long long 바디크기 = msg.바디크기;
        sendBuffer.insert(sendBuffer.end(), reinterpret_cast<char*>(&바디크기), reinterpret_cast<char*>(&바디크기) + sizeof(unsigned long long));

        sendBuffer.insert(sendBuffer.end(), msg.strBodyMsg.begin(), msg.strBodyMsg.end());

        // 데이터 전송
        int totalSent = 0;
        int toSend = static_cast<int>(sendBuffer.size());
        while (totalSent < toSend) {
            int sent = send(m_clientSocket, sendBuffer.data() + totalSent, toSend - totalSent, 0);
            if (sent == SOCKET_ERROR) {
                std::cerr << "전송 실패: " << WSAGetLastError() << "\n";
                cleanup();
                return;
            }
            totalSent += sent;
        }
        printf("\n==전송 완료==\n");
    }
}

// 수신 시작 함수
void CTCPClient::startReceiving()
{
    m_recvThread = std::thread(&CTCPClient::recvMsg, this);
    m_recvThread.detach();
}

// 수신 중지 함수
void CTCPClient::stopReceiving()
{
    m_isConnected = false;
}

// 수신 데이터 처리 함수
void CTCPClient::recvMsg()
{
    std::vector<char> vecBuf(BUFFER_SIZE);
    int nResult = -1;
    while (m_isConnected) {
        // 헤더 크기(14바이트) 이상인지 확인
        int nTotalRecvSize = 0;

        // 헤더 수신
        while (nTotalRecvSize < HEADER_SIZE) {
            nResult = recv(m_clientSocket, vecBuf.data() + nTotalRecvSize, HEADER_SIZE - nTotalRecvSize, 0);
            if (nResult > 0) {
                nTotalRecvSize += nResult;
            }
            else if (nResult == 0) {
                std::cerr << "서버와의 연결이 끊겼습니다\n";
                m_isConnected = false;
                return;
            }
            else {
                std::cerr << "수신 실패: " << WSAGetLastError() << "\n";
                m_isConnected = false;
                return;
            }
        }

        // 헤더 파싱
        ST_TCPMsg msg = { 0, };
        msg.송신자 = vecBuf[0];
        msg.수신자 = vecBuf[1];
        msg.메시지아이디 = *reinterpret_cast<int*>(vecBuf.data() + 2);
        msg.바디크기 = *reinterpret_cast<unsigned long long*>(vecBuf.data() + 6);

        // 바디 수신
        nTotalRecvSize = 0;
        while (nTotalRecvSize < static_cast<int>(msg.바디크기)) 
        {
            nResult = recv(m_clientSocket, vecBuf.data() + HEADER_SIZE + nTotalRecvSize, msg.바디크기 - nTotalRecvSize, 0);
            if (nResult > 0) {
                nTotalRecvSize += nResult;
            }
            else if (nResult == 0) {
                std::cerr << "서버 연결 끊김\n";
                m_isConnected = false;
                return;
            }
            else {
                std::cerr << "수신 실패: " << WSAGetLastError() << "\n";
                m_isConnected = false;
                return;
            }
        }

        msg.strBodyMsg.assign(vecBuf.begin() + HEADER_SIZE, vecBuf.begin() + HEADER_SIZE + msg.바디크기);

        // 세이프큐에 넣는다

        // 정상 수신 완료 (디버깅)
        printf("서버전송: %s\n", msg.strBodyMsg.c_str());
    }
}

void CTCPClient::cleanup()
{
    if (m_isConnected) {
        closesocket(m_clientSocket);
        WSACleanup();
        m_isConnected = false;
    }
}

int main() {
    CTCPClient client("127.0.0.1", 8888);

    if (client.initialize()) {
        client.startReceiving();

        std::string inputMessage;
        while (true) {
            std::cout << "\n서버에 보낼 메시지를 입력하세요:\n";
            std::getline(std::cin, inputMessage);
            if (inputMessage == "exit") {
                break;
            }
            client.sendMsg(inputMessage);              // 헤더를 붙여서 바이트로 전송한다
        }
    }

    client.stopReceiving();

    return 0;
}