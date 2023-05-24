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
#define MAX_USER            200

// ������ ������ Ŭ���̾�Ʈ ����
struct User
{
    SOCKET socket;                 // Ŭ���̾�Ʈ ����
    TCHAR ip[16];                   // Ŭ���̾�Ʈ �ּ�
};

class AsyncServer
{
public:
    // ������
    explicit AsyncServer()
    {
        WSADATA wsaData;
        (void)WSAStartup(0x0202, &wsaData);                     // ���̺귯�� ����

        for (int i = 0; i < MAX_USER; i++)
        {
            m_user[i].socket = INVALID_SOCKET;
        }
        m_server_socket = INVALID_SOCKET;
    }

    // �ı���
    virtual ~AsyncServer()
    {
        User* p = m_user;
        for (int i = 0; i < MAX_USER; i++, p++)
        {
            if (p->socket != INVALID_SOCKET) {
                closesocket(p->socket);
                p->socket = INVALID_SOCKET;
            }
        }

        if (m_server_socket != INVALID_SOCKET) {
            closesocket(m_server_socket);
            m_server_socket = INVALID_SOCKET;
        }

        WSACleanup();
    }

    // ������ �����ϴ� �Լ� (�񵿱�)
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

    void AcceptClient()
    {
        while (true) {
            // Ŭ���̾�Ʈ ����
            sockaddr_in client_address;
            int client_address_size = sizeof(client_address);

            // ���� ����
            SOCKET client_socket = accept(m_server_socket, (sockaddr*)&client_address, &client_address_size);

            TCHAR ip_address[16];
            InetNtop(AF_INET, &client_address.sin_addr, ip_address, 16);
            _tprintf(_T("Ŭ���̾�Ʈ ���� %s\n"), ip_address);

            for (int i = 0; i < MAX_USER; i++)
            {
                if (m_user[i].socket == INVALID_SOCKET)
                {
                    m_user[i].socket = client_socket;
                    _tcscpy_s(m_user[i].ip, 16, ip_address);

                    // �񵿱�� �ش� Ŭ���̾�Ʈ�� ���� recv ����
                    m_recv_future = std::async(std::launch::async, &AsyncServer::RecvMsg, this, client_socket);
                    break;
                }
            }
        }
    }

    // �޽��� ���� (�񵿱�)
    void RecvMsg(SOCKET clientSocket)
    {
        while (true)
        {
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            TCHAR wbuffer[1024] = { 0, };

            // �޽��� ����
            int read = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (read > 0)
            {
                A2Wpchar(buffer, wbuffer);
                User* p = m_user;
                TCHAR ip[16];
                for (int i = 0; i < MAX_USER; i++)
                {
                    if (p->socket == clientSocket)
                    {
                        memcpy(ip, p->ip, sizeof(p->ip));
                    }
                }
                _tprintf(_T("\n %s : %s \n"), ip, wbuffer);
            }
            else if (read == 0)
            {
                printf("Ŭ���̾�Ʈ ���� ���� \n");
                break;
            }
            else if(read == SOCKET_ERROR)
            {
                printf("Ŭ���̾�Ʈ ���� ���� \n");
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

    void CloseSocketClient(SOCKET a_client_socket)
    {
        User* p = m_user;
        User* target_client = NULL;

        // �ش� ������ ã��
        for (int i = 0; i < MAX_USER; i++, p++)
        {
            if(p->socket == a_client_socket)
            {
                target_client = m_user + i;
                closesocket(a_client_socket);               // �����Ų��
            }
        }
        
        // ������ �����Ѵ�
        if (target_client != NULL)
        {
            target_client->socket = INVALID_SOCKET;
            memset(target_client->ip, 0, sizeof(target_client->ip));
        }
    }

    // �޽��� ��� ����ڿ��� ������
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

    // �޽��� ������
    void SendToClient(SOCKET a_socket, const char* a_msg)
    {
        int msg_len = strlen(a_msg);
        char* data = new char[msg_len];
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
    User m_user[MAX_USER];
    SOCKET m_server_socket = INVALID_SOCKET;
    sockaddr_in m_server_address;

    std::future<void> m_accept_future;
    std::future<void> m_recv_future;
    std::future<void> m_send_future;
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

        // ��ü�� �޼��带 �񵿱� ó��
        std::future<void> future = std::async(std::launch::async, std::bind(&AsyncServer::SendBroadcast, &asyncServer, server_msg));
    }


}

