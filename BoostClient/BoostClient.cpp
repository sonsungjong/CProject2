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
    SocketClient(const std::string& server, const std::string& port) : io_context(), m_socket(io_context)
    {
        tcp::resolver resolver(io_context);
        boost::asio::connect(m_socket, resolver.resolve(server, port));
    }

    void receive()
    {
        while (1)
        {
            char buf[10000] = { 0 };
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
            printf("\n==�����κ��͹��� ������ :%d==\n", m_recv_msg.size());

            // ��ȯ
            //TCHAR wmsg[10000] = { 0 };
            //int origin_len = (int)strlen(m_recv_msg.c_str());
            //int ilen = MultiByteToWideChar(CP_ACP, 0, m_recv_msg.c_str(), origin_len, NULL, NULL);
            //MultiByteToWideChar(CP_ACP, 0, m_recv_msg.c_str(), origin_len, wmsg, ilen);

            //TestRecvPrint(wmsg);            // �׽�Ʈ ���� ���
        }
    }               // receive()

    void send(const std::string& msg)
    {
        std::cout << "������ ����: " << msg << std::endl;
        boost::asio::write(m_socket, boost::asio::buffer(msg));
    }           // send()

    void TestRecvPrint(TCHAR* wmsg)
    {
        _tprintf(_T("��������>> %s \n"), wmsg);
    }           // TestRecvPrint()

private:
    boost::asio::io_context io_context;
    tcp::socket m_socket;
    std::string m_recv_msg;
    
};

int main()
{
    _tsetlocale(0, _T(""));

    std::string server_ip = "127.0.0.1";
    std::string server_port = "9004";

    SocketClient client(server_ip, server_port);

    std::thread receive_thread(&SocketClient::receive, &client);

    TCHAR w_msg[10000] = { 0, };
    char msg[10000] = { 0, };

    while (1) {
        printf("�Է�>>");
        rewind(stdin);
        memset(msg, 0, sizeof(msg));
        memset(w_msg, 0, sizeof(w_msg));
        _tscanf_s(_T("%[^\n]s"), w_msg, 10000);
        int len = WideCharToMultiByte(CP_ACP, 0, w_msg, -1, NULL, 0, NULL, NULL);
        WideCharToMultiByte(CP_ACP, 0, w_msg, -1, msg, len, NULL, NULL);

        client.send(msg);
    }

    receive_thread.join();
}

