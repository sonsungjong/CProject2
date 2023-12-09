/*
    TCP/IP ����� ���ؼ�
    ��ȭ���� �����ϰų�
    �ش� ���α׷��� ��Ʈ�� ���� Windows ��ȭ�� ���ܸ� �߰��ؾ��Ѵ�.

    ������ -> �ý��� �� ���� -> Windows Defender ��ȭ�� -> ��� ���� -> ��� ������ ���Ե� Windows Defender ��ȭ��
    -> �ιٿ�� ��Ģ Ŭ�� -> �� ��Ģ Ŭ�� -> ���α׷� �Ǵ� ��Ʈ�� �����ϰ� ����
    -> �ƿ��ٿ�� ��Ģ ����(������ ����) -> �� ��Ģ Ŭ�� -> ���α׷� �Ǵ� ��Ʈ�� �����ϰ� ����
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <tchar.h>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <WinSock2.h>
#include <WS2tcpip.h>

// ���̺귯�� ��ũ
#pragma comment(lib, "WS2_32.lib")

#define MAX_USER            2000
#define PORT                    8888

// ������ ������ Ŭ���̾�Ʈ ����
struct User
{
    SOCKET socket;                 // Ŭ���̾�Ʈ ����
    TCHAR ip[16];                   // Ŭ���̾�Ʈ �ּ�
    std::thread recv_thread;           // �ش� Ŭ���̾�Ʈ�� ��ȭ�� �� �ִ� ������
    std::atomic<bool> running;              // ������ �������� ����
};

class AsyncServer
{
public:
    // ������
    explicit AsyncServer()
    {
        WSADATA wsaData;
        (void)WSAStartup(0x0202, &wsaData);                     // ���̺귯�� ����
        m_user = new User[MAX_USER];
        User* p = m_user;

        for (int i = 0; i < MAX_USER; i++, p++)
        {
            p->socket = INVALID_SOCKET;
            memset(p->ip, 0, sizeof(p->ip));
            p->running = false;
        }
        m_server_socket = INVALID_SOCKET;
        m_accept_flag = true;

        m_recv_msg = new char[max_buffer_size] {0};
        m_recv_wmsg = new wchar_t[max_buffer_size] {0};
        m_response_to_client = new char[max_buffer_size] {0};
    }

    // �ı���
    virtual ~AsyncServer()
    {
        SendBroadcast("====������ ����Ǿ����ϴ�====");
        CloseAllUser();

        if (m_server_socket != INVALID_SOCKET) {
            m_accept_flag = false;
            m_accept_future.wait();
            closesocket(m_server_socket);
            m_server_socket = INVALID_SOCKET;
        }

        WSACleanup();

        delete[] m_recv_msg;
        delete[] m_recv_wmsg;
        delete[] m_response_to_client;
    }

    /*
        ���� ����
        �񵿱�� accept �� ����
    */
    void StartListen(int a_port)
    {
        // ���� ����
        m_server_socket = socket(AF_INET, SOCK_STREAM, 0);

        m_server_address.sin_family = AF_INET;
        m_server_address.sin_port = htons(a_port);
        m_server_address.sin_addr.S_un.S_addr = INADDR_ANY;

        // ��Ʈ��ũ ����
        bind(m_server_socket, (sockaddr*)&m_server_address, sizeof(m_server_address));
        
        // ���� ����
        listen(m_server_socket, 5);

        // �񵿱�� accept() ����
        m_accept_future = std::async(std::launch::async, &AsyncServer::AcceptClient, this);
    }

    /*
        Ŭ���̾�Ʈ ������ ���� �Լ�
        �� Ŭ���̾�Ʈ���� �߰� �����带 �����Ͽ� recv
        (������ �������� �۾�ť�� ������Ǯ ���·� �ٲ㼭 ���ҽ��� �����ϴ°� ����)
    */
    void AcceptClient()
    {
        while (m_accept_flag == true) {
            // Ŭ���̾�Ʈ ����
            sockaddr_in client_address;
            int client_address_size = sizeof(client_address);

            // ���� ����
            SOCKET client_socket = accept(m_server_socket, (sockaddr*)&client_address, &client_address_size);

            TCHAR ip_address[16];
            InetNtop(AF_INET, &client_address.sin_addr, ip_address, 16);
            _tprintf(_T("Ŭ���̾�Ʈ ���� %s\n"), ip_address);

            User* p = m_user;

            for (int i = 0; i < MAX_USER; i++, p++)
            {
                if (p->socket == INVALID_SOCKET)
                {
                    p->socket = client_socket;
                    _tcscpy_s(p->ip, 16, ip_address);
                    p->running = true;

                    // �߰� �����带 ���� �ش� Ŭ���̾�Ʈ�� ���� recv ����
                    p->recv_thread = std::thread(&AsyncServer::RecvMsg, this, client_socket);
                    break;
                }
            }
        }
    }

    /*
        �޽��� ����
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
            memset(m_recv_msg, 0, max_buffer_size);
            memset(m_recv_wmsg, 0, max_buffer_size);
            memset(m_response_to_client, 0, max_buffer_size+16);
            char ip[16] = { 0 };

            // �޽��� ����
            int read = recv(clientSocket, m_recv_msg, max_buffer_size -1, 0);
            if (read > 0)
            {
                m_recv_msg[read] = '\0';
                A2Wpchar(m_recv_msg, m_recv_wmsg);
                _tprintf(_T("\n %s : %s \n"), target_user->ip, m_recv_wmsg);

                W2Apchar(target_user->ip, ip);
                sprintf_s(m_response_to_client, read+17, "%s : %s", ip, m_recv_msg);
                SendBroadcast(m_response_to_client);
            }
            else if (read == 0)
            {
                printf("Ŭ���̾�Ʈ ���� ���� \n");
                break;
            }
            else if(read == SOCKET_ERROR)
            {
                printf("Ŭ���̾�Ʈ ���α׷� ���� \n");
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
        Ư�� Ŭ���̾�Ʈ ���� ����
    */
    void CloseSocketClient(SOCKET a_client_socket)
    {
        User* p = m_user;

        // �ش� ������ ã��
        for (int i = 0; i < MAX_USER; i++, p++)
        {
            if(p->socket == a_client_socket)
            {
                p->running = false;
                closesocket(a_client_socket);               // �����Ų��
                memset(p->ip, 0, sizeof(p->ip));
                p->socket = INVALID_SOCKET;
                p->recv_thread.detach();
            }
        }
    }

    /*
        ��� Ŭ���̾�Ʈ ���� ����
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
        ��� Ŭ���̾�Ʈ���� �޽��� ������
    */
    void SendBroadcast(const char* a_msg)
    {
        int msg_len = strlen(a_msg);
        char* data = new char[msg_len+1];
        memset(data, 0, msg_len+1);
        memcpy(data, a_msg, msg_len);

        User* p = m_user;
        for (int i = 0; i < MAX_USER; i++, p++)
        {
            if (p->socket != INVALID_SOCKET) {
                send(p->socket, data, msg_len, 0);
                //printf("\n���̰ź���>>%s<<\n", data);
            }
        }
        delete[] data;
    }

    /*
        Ư�� Ŭ���̾�Ʈ���� �޽��� ������
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

    // UTF-8�� ��� CP_ACP�� CP_UTF8�� �����ϱ�

    /*
    ��Ƽ����Ʈ���� �����ڵ�� ��ȯ (char)
    */
    void A2Wpchar(const char* const p_char, wchar_t* const p_wchar)
    {
        int origin_len = strlen(p_char);
        int len = MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, NULL, NULL);
        MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, p_wchar, len);
    }

    /*
    �����ڵ忡�� ��Ƽ����Ʈ�� ��ȯ (char)
    */
    void W2Apchar(const wchar_t* const p_wchar, char* const p_char)
    {
        int len = WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, NULL, 0, NULL, NULL);
        WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, p_char, len, NULL, NULL);
    }

private:
    User* m_user;
    SOCKET m_server_socket = INVALID_SOCKET;
    sockaddr_in m_server_address;

    std::future<void> m_accept_future;
    std::atomic<bool> m_accept_flag;

    char* m_recv_msg;
    wchar_t* m_recv_wmsg;
    char* m_response_to_client;
    const int max_buffer_size = 16384;
};

int main()
{
    _tsetlocale(0, _T("korean"));
    char server_msg[256] = { 0, };
    AsyncServer asyncServer;

    asyncServer.StartListen(8888);

    while (true)
    {
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        printf("Ŭ���̾�Ʈ ��ü�� ���� �޽���>>");
        rewind(stdin);
        scanf_s("%[^\n]s", server_msg, 256);

        // ��ü�� Send �޼��带 �񵿱� ó��
        std::future<void> future = std::async(std::launch::async, std::bind(&AsyncServer::SendBroadcast, &asyncServer, server_msg));
    }


}

