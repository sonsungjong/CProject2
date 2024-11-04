#include "pch.h"
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 255

#pragma pack(push, 1)
typedef struct SetMergeInfo 
{
    int output;
    int width;
    int height;
    int x;
    int y;
} ST_SetMergeInfo;
#pragma pack(pop)

class CTCPClient
{
public:
    CTCPClient(const std::string& _strIP, unsigned short _usPort);
    virtual ~CTCPClient();

    bool initialize();
    void sendMsg(const char* msg);
    void startReceiving();
    void stopReceiving();

    void recvMsg();
    void cleanup();

    void SetMerge(ST_SetMergeInfo infos[], int numMerges)
    {
        char temp[MAX_BUFFER_SIZE]{ 0};
        char buffer[50]{ 0 };

        for (int i = 0; i < numMerges; i++) {
            sprintf_s(buffer, sizeof(buffer), "[%03d-%03d,%03d,%03d,%03d]",
                infos[i].output,
                infos[i].width,
                infos[i].height,
                infos[i].x,
                infos[i].y);

            strncat_s(temp, sizeof(temp), buffer, sizeof(temp) - strlen(temp) - 1);
        }

        char result[MAX_BUFFER_SIZE]{ 0 };
        sprintf_s(result, sizeof(result), "*255sOE%s!", temp);
        sendMsg(result);
    }

private:
    std::string m_serverIP;
    unsigned short m_serverPort;
    unsigned long long m_clientSocket;
    bool m_isConnected;
    std::thread m_recvThread;


};

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

void CTCPClient::sendMsg(const char* msg)
{
    if (m_isConnected) {

        // 데이터 전송
        int sent = send(m_clientSocket, msg, static_cast<int>(strlen(msg)), 0);
        if (sent == SOCKET_ERROR) {
            std::cerr << "전송 실패: " << WSAGetLastError() << "\n";
            cleanup();
            return;
        }
        
        printf("\n==전송 완료: %s==\n", msg);
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
    char buf[MAX_BUFFER_SIZE]{ 0 };
    int nResult = -1;
    while (m_isConnected) {
        // 헤더 수신
        
        nResult = recv(m_clientSocket, buf, sizeof(buf), 0);
        if (nResult > 0) {
            // 세이프 큐에 넣는다

            // 디버깅
            printf("응답: %s\n", buf);
        }
        else if (nResult == 0) {
            std::cerr << "서버와의 연결이 끊겼습니다\n";
            m_isConnected = false;
        }
        else {
            std::cerr << "수신 실패: " << WSAGetLastError() << "\n";
            m_isConnected = false;
        }
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
    CTCPClient client("192.168.101.3", 3000);

    if (client.initialize()) {
        client.startReceiving();

        std::string inputMsg;
        while (true) {
            std::cout << "\n서버에 보낼 메시지를 입력하세요:\n";
            std::getline(std::cin, inputMsg);
            if (inputMsg == "exit") {
                break;
            }
            else if (inputMsg == "1")
            {
                // GET DEVICE Info
                char buf[] = "*255dh!";
                client.sendMsg(buf);
            }
            else if (inputMsg == "2") {
                // GET Version
                char buf[] = "*255?VERSION!";
                client.sendMsg(buf);
            }
            else if (inputMsg == "3") {
                // GET Binding info
                char buf[] = "*255?ALLIO!";
                client.sendMsg(buf); 
            }
            else if (inputMsg == "4") {
                // Set Binding info
                char buf[] = "*255CI001O001,I005O002,I009O003,I001O004,I005O005,I009O006,I013O007,I014O008!";
                client.sendMsg(buf);
            }
            else if (inputMsg == "5") {
                // Set Merge (sOE)
                ST_SetMergeInfo infos[] = {
                    {1, 200, 100, 10, 20},
                    {2, 300, 150, 30, 40}
                };
                int numMerges = sizeof(infos) / sizeof(infos[0]);
                client.SetMerge(infos, numMerges);
            }
            else if (inputMsg == "6") {
                // Set Separate (sOD)

            }
        }
    }

    client.stopReceiving();

    return 0;
}