/*
    TCP/IP 통신을 위해선
    방화벽을 해제하거나
    해당 프로그램과 포트에 대해 Windows 방화벽 예외를 추가해야한다.

    제어판 -> 시스템 및 보안 -> Windows Defender 방화벽 -> 고급 설정 -> 고급 보안이 포함된 Windows Defender 방화벽
    -> 인바운드 규칙 클릭 -> 새 규칙 클릭 -> 프로그램 또는 포트를 선택하고 다음
    -> 아웃바운드 규칙 생성(나가는 연결) -> 새 규칙 클릭 -> 프로그램 또는 포트를 선택하고 다음
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <tchar.h>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

// 라이브러리 링크
#pragma comment(lib, "WS2_32.lib")
#define MAX_USER            200
#define PORT 18888

// 서버에 접속한 클라이언트 정보
struct User
{
    SOCKET socket;                 // 클라이언트 소켓
    TCHAR ip[16];                   // 클라이언트 주소
    std::thread recv_thread;           // 해당 클라이언트와 대화할 수 있는 스레드
    std::atomic<bool> running;              // 스레드 실행종료 여부
};

class AsyncServer
{
public:
    // 생성자
    explicit AsyncServer()
    {
        WSADATA wsaData;
        (void)WSAStartup(0x0202, &wsaData);                     // 라이브러리 실행
        User* p = m_user;

        for (int i = 0; i < MAX_USER; i++, p++)
        {
            p->socket = INVALID_SOCKET;
            memset(p->ip, 0, sizeof(p->ip));
            p->running = false;
        }
        m_server_socket = INVALID_SOCKET;
        m_accept_flag = true;
    }

    // 파괴자
    virtual ~AsyncServer()
    {
        //SendBroadcast("====서버가 종료되었습니다====");
        CloseAllUser();

        if (m_server_socket != INVALID_SOCKET) {
            m_accept_flag = false;
            m_accept_future.wait();
            closesocket(m_server_socket);
            m_server_socket = INVALID_SOCKET;
        }

        WSACleanup();
    }

    /*
        서버 시작
        비동기로 accept 를 실행
    */
    void StartListen(int a_port)
    {
        // 소켓 생성
        m_server_socket = socket(AF_INET, SOCK_STREAM, 0);

        m_server_address.sin_family = AF_INET;
        m_server_address.sin_port = htons(a_port);
        m_server_address.sin_addr.S_un.S_addr = INADDR_ANY;

        // 네트워크 연결
        bind(m_server_socket, (sockaddr*)&m_server_address, sizeof(m_server_address));
        
        // 서버 시작
        listen(m_server_socket, 5);

        // 비동기로 accept() 실행
        m_accept_future = std::async(std::launch::async, &AsyncServer::AcceptClient, this);
    }

    /*
        클라이언트 접속을 위한 함수
        각 클라이언트마다 추가 스레드를 생성하여 recv
        (유저가 많아지면 작업큐와 스레드풀 형태로 바꿔서 리소스를 절약하는게 좋음)
    */
    void AcceptClient()
    {
        while (m_accept_flag == true) {
            // 클라이언트 정보
            sockaddr_in client_address;
            int client_address_size = sizeof(client_address);

            // 연결 수락
            SOCKET client_socket = accept(m_server_socket, (sockaddr*)&client_address, &client_address_size);

            TCHAR ip_address[16];
            InetNtop(AF_INET, &client_address.sin_addr, ip_address, 16);
            _tprintf(_T("클라이언트 접속 %s\n"), ip_address);

            User* p = m_user;

            for (int i = 0; i < MAX_USER; i++, p++)
            {
                if (p->socket == INVALID_SOCKET)
                {
                    p->socket = client_socket;
                    _tcscpy_s(p->ip, 16, ip_address);
                    p->running = true;

                    // 추가 스레드를 통해 해당 클라이언트에 대해 recv 실행
                    p->recv_thread = std::thread(&AsyncServer::RecvMsg, this, client_socket);
                    break;
                }
            }
        }
    }

    /*
        메시지 수신
    */
    void RecvMsg(SOCKET clientSocket)
    {
        User* p = m_user;
        User* target_user = NULL;

        for (int i = 0; i < MAX_USER; i++, p++)
        {
            if (p->socket == clientSocket)
            {
                target_user = m_user + i;
            }
        }

        while (target_user->running == true)
        {
            char buffer[1024];
            TCHAR wbuffer[1024] = { 0, };
            memset(buffer, 0, sizeof(buffer));
            memset(wbuffer, 0, sizeof(wbuffer));
            char ip[16] = { 0, };
            char respon[2000] = { 0, };

            // 메시지 수신
            int read = recv(clientSocket, buffer, sizeof(buffer)-1, 0);
            if (read > 0 && strcmp(buffer, "time") == 0)
            {
                //A2Wpchar(buffer, wbuffer);
               // _tprintf(_T("\n %s : %s \n"), target_user->ip, wbuffer);

                //W2Apchar(target_user->ip, ip);
                //sprintf_s(respon, 2000, "%s : %s", ip, buffer);
                SendBroadcast(respon);
            }
            else if (read == 0)
            {
                printf("클라이언트 정상 종료 \n");
                break;
            }
            else if(read == SOCKET_ERROR)
            {
                printf("클라이언트 프로그램 종료 \n");
                break;
            }
            else
            {
                printf("Error\n");
                break;
            }
        }
        CloseSocketClient(clientSocket);
    }

    /*
        특정 클라이언트 접속 해제
    */
    void CloseSocketClient(SOCKET a_client_socket)
    {
        User* p = m_user;

        // 해당 유저를 찾아
        for (int i = 0; i < MAX_USER; i++, p++)
        {
            if(p->socket == a_client_socket)
            {
                p->running = false;
                closesocket(a_client_socket);               // 종료시킨다
                memset(p->ip, 0, sizeof(p->ip));
                p->socket = INVALID_SOCKET;
                p->recv_thread.detach();
            }
        }
    }

    /*
        모든 클라이언트 접속 해제
    */
    void CloseAllUser()
    {
        User* p = m_user;

        for (int i = 0; i < MAX_USER; i++, p++)
        {
            if (p->socket != INVALID_SOCKET) {
                p->running = false;
                closesocket(p->socket);
                memset(p->ip, 0, sizeof(p->ip));
                p->socket = INVALID_SOCKET;
                p->recv_thread.detach();
            }
        }
    }
    

    /*
        모든 클라이언트에게 메시지 보내기
    */
    void SendBroadcast(const char* a_msg)
    {
        // 현재 시간을 얻는다
        SYSTEMTIME st;
        GetLocalTime(&st);              // UTC가 아닌 로컬타임으로 사용한다
        char datetime[20] = { 0, };                         // YYYY-MM-DD HH:mm:SS
        int msg_len = sprintf_s(datetime, 20, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

        // 보낸다
        User* p = m_user;
        for (int i = 0; i < MAX_USER; i++, p++)
        {
            if (p->socket != INVALID_SOCKET) {
                send(p->socket, datetime, msg_len + 1, 0);
            }
        }
    }

    /*
        특정 클라이언트에게 메시지 보내기
    */
    void SendToClient(SOCKET a_socket, const char* a_msg)
    {
        int msg_len = strlen(a_msg);
        char* data = new char[msg_len + 1];
        memset(data, 0, msg_len + 1);
        memcpy(data, a_msg, msg_len);

        send(a_socket, data, msg_len, 0);
        delete[] data;
    }

    // UTF-8일 경우 CP_ACP를 CP_UTF8로 변경하기

    /*
    멀티바이트에서 유니코드로 변환 (char)
    */
    void A2Wpchar(const char* const p_char, wchar_t* const p_wchar)
    {
        int origin_len = strlen(p_char);
        int len = MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, NULL, NULL);
        MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, p_wchar, len);
    }

    /*
    유니코드에서 멀티바이트로 변환 (char)
    */
    void W2Apchar(const wchar_t* const p_wchar, char* const p_char)
    {
        int len = WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, NULL, 0, NULL, NULL);
        WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, p_char, len, NULL, NULL);
    }

private:
    User m_user[MAX_USER];
    SOCKET m_server_socket = INVALID_SOCKET;
    sockaddr_in m_server_address;

    std::future<void> m_accept_future;
    std::atomic<bool> m_accept_flag;
};

int main()
{
    _tsetlocale(0, _T(""));
    //HWND hWnd = GetConsoleWindow();
    //ShowWindow(hWnd, SW_HIDE); // SW_HIDE는 창을 숨기는 상수입니다.

    char server_msg[256] = { 0, };
    AsyncServer asyncServer;

    asyncServer.StartListen(PORT);

    while (true)
    {
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        printf("클라이언트 전체에 보낼 메시지>>");
        rewind(stdin);
        scanf_s("%[^\n]s", server_msg, 256);

        // 객체의 Send 메서드를 비동기 처리
        //std::future<void> future = std::async(std::launch::async, std::bind(&AsyncServer::SendBroadcast, &asyncServer, server_msg));
    }


}

