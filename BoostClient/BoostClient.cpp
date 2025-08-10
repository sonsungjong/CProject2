// Boost ���̺귯�� 1.83.0 ����
/*
��ġ
https://www.boost.org/ -> Download -> boost_1_83_0.zip -> ��������
boost_1_83_0 ���� -> ������ �������� cmd ���� -> .\bootstrap.bat �Է��Ͽ� ����
.\b2.exe �Է��Ͽ� ����

���� ���丮 : C:\boost_1_83_0
���̺귯�� ���丮 : C:\boost_1_83_0\stage\lib
�ʿ��� ���̺귯���� �Է¿� �־����

���̺귯�� �Է�(������64) : libboost_system-vc142-mt-x64-1_83.lib
���̺귯�� �Է�(�����64) : libboost_system-vc142-mt-gd-x64-1_83.lib
���̺귯�� �Է�(������32) : libboost_system-vc142-mt-x32-1_83.lib
���̺귯�� �Է�(�����32) : libboost_system-vc142-mt-gd-x32-1_83.lib
*/
#include <cstdio>
#include <iostream>
#include <thread>
#include <tchar.h>
#include <boost/asio.hpp>
#include <boost/locale.hpp>

#ifdef _DEBUG
#pragma comment(lib, "libboost_system-vc142-mt-gd-x64-1_83.lib")
#pragma comment(lib, "libboost_locale-vc142-mt-gd-x64-1_83.lib")
#else
#pragma comment(lib, "libboost_system-vc142-mt-x64-1_83.lib")
#pragma comment(lib, "libboost_locale-vc142-mt-x64-1_83.lib")
#endif

using boost::asio::ip::tcp;

class SocketClient
{
public:
    SocketClient(const std::string& server, const std::string& port)
        : m_io_context()
        , m_socket(m_io_context)
        , m_server(server)
        , m_port(port)
    {
        //tcp::resolver resolver(io_context);
        //boost::asio::connect(m_socket, resolver.resolve(server, port));
        connectWithRetry();
        m_thread_receive = std::thread(&SocketClient::receive, this);
    }

    virtual ~SocketClient()
    {
        m_io_context.stop();
        if (m_thread_receive.joinable()) {
            m_thread_receive.join();
        }
    }

    void connectWithRetry()
    {
        tcp::resolver resolver(m_io_context);
        while (true)
        {
            try {
                auto endpoints = resolver.resolve(m_server, m_port);
                boost::asio::connect(m_socket, endpoints);
                printf("���� ���� ����: %s:%s\n", m_server.c_str(), m_port.c_str());
                break;
            }
            catch (const std::exception& e)
            {
                printf("���� ���� ����: %s (%s:%s). 1�� �� ��õ��մϴ�...\n", e.what(), m_server.c_str(), m_port.c_str());
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }

    void reconnect()
    {
        boost::system::error_code ec;
        m_socket.close(ec);
        if (ec) { printf("���� close ����: %s\n", ec.message().c_str()); }
        connectWithRetry();
    }

    void receive()
    {
        while (1)
        {
            try {
                char buf[1024] = { 0 };
                boost::system::error_code err;

                size_t len = m_socket.read_some(boost::asio::buffer(buf), err);

                if (err == boost::asio::error::eof)
                {
                    break;
                }
                else if (err)
                {
                    throw boost::system::system_error(err);
                }

                // ���� �޽����� ������ ����
                m_recv_msg = std::string(buf, len);
                printf("\n==�����κ��͹����� :%s==\n", m_recv_msg.c_str());

                // ��ȯ
                TCHAR wmsg[1024] = { 0 };
                int origin_len = (int)strlen(m_recv_msg.c_str());
                int ilen = MultiByteToWideChar(CP_ACP, 0, m_recv_msg.c_str(), origin_len, NULL, NULL);
                MultiByteToWideChar(CP_ACP, 0, m_recv_msg.c_str(), origin_len, wmsg, ilen);

                TestRecvPrint(wmsg);            // �׽�Ʈ ���� ���
            }
            catch (const boost::system::system_error& e)
            {
                printf("���� ���� �߻�: %s. 1�� �� �翬��...\n", e.what());
                std::this_thread::sleep_for(std::chrono::seconds(1));
                reconnect();
            }
            catch (const std::exception& e)
            {
                printf("���� �߻�: %s. 1�� �� �翬��...\n", e.what());
                std::this_thread::sleep_for(std::chrono::seconds(1));
                reconnect();
            }
            catch (...)
            {
                printf("�� �� ���� ���� �߻�, 1�� �� �翬��...\n");
                std::this_thread::sleep_for(std::chrono::seconds(1));
                reconnect();
            }
        }
    }               // receive()

    void send(const std::string& msg)
    {
        try {
            std::cout << "������ ����: " << msg << std::endl;
            boost::asio::write(m_socket, boost::asio::buffer(msg));
        }
        catch (const std::exception& e) {
            printf("Send ����: %s. �翬�� �õ��մϴ�...\n", e.what());
            reconnect();
            // ��õ��� ����: �ʿ��ϸ� send(msg) ��� ȣ���ϰų� ť�� ���� �� ������
        }
    }           // send()

    void TestRecvPrint(TCHAR* wmsg)
    {
        _tprintf(_T("��������>> %s \n"), wmsg);
    }           // TestRecvPrint()

private:
    boost::asio::io_context m_io_context;
    tcp::socket m_socket;
    std::string m_recv_msg;
    std::string m_server;
    std::string m_port;
    std::thread m_thread_receive;
};

int main()
{
    _tsetlocale(0, _T(""));

    std::string server = "192.168.0.114";
    std::string port = "9004";

    SocketClient client(server, port);

    TCHAR w_msg[1024] = { 0, };
    char msg[1024] = { 0, };

    while (1) {
        printf("�Է�>>");
        rewind(stdin);
        memset(msg, 0, sizeof(msg));
        memset(w_msg, 0, sizeof(w_msg));
        _tscanf_s(_T("%[^\n]s"), w_msg, 1024);
        int len = WideCharToMultiByte(CP_ACP, 0, w_msg, -1, NULL, 0, NULL, NULL);
        WideCharToMultiByte(CP_ACP, 0, w_msg, -1, msg, len, NULL, NULL);

        client.send(msg);
    }
}

