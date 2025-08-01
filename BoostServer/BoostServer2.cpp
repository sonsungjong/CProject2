/*
Boost ���̺귯�� 1.83.0 ���� ��ġ
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
#include <iostream>
#include <deque>
#include <memory>
#include <set>
#include <unordered_map>
#include <thread>
#include <boost/asio.hpp>
#include <boost/locale.hpp>

// asio
// utf8
// 
#ifdef _DEBUG
#pragma comment(lib, "libboost_system-vc142-mt-gd-x64-1_83.lib")
#pragma comment(lib, "libboost_locale-vc142-mt-gd-x64-1_83.lib")
#else
#pragma comment(lib, "libboost_system-vc142-mt-x64-1_83.lib")
#pragma comment(lib, "libboost_locale-vc142-mt-x64-1_83.lib")
#endif

const int DEFAULT_PORT = 9004;


class CTCPServer {
public:
    CTCPServer(short port)
        : m_io_context()
        , m_acceptor(m_io_context)
    {
        m_port = port;
        boost::system::error_code ec;
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), m_port);
        m_acceptor.open(endpoint.protocol(), ec);

        if (!ec) {
            m_acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);

            if (!ec) {
                m_acceptor.bind(endpoint, ec);

                if (!ec) {
                    m_acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);

                    if (!ec) {

                    }
                }
            }
        }

        startAccept();
    }

    virtual ~CTCPServer() {
        stopServer();
    }

    // TCP ���� ����
    void startServer() {
        m_io_context.run();
    }

    // TCP ���� ����
    void stopServer() {
        m_io_context.stop();
    }

    // Ŭ���̾�Ʈ ������Ʈ ����
    void startAccept() {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_io_context);
        m_acceptor.async_accept(*socket, [this, socket](boost::system::error_code ec) {
            if (!ec) {
                auto endpoint = socket->remote_endpoint();
                unsigned short client_port = endpoint.port();
                m_clients[client_port] = socket;

                std::cout << "Ŭ���̾�Ʈ ����: ��Ʈ " << client_port << " (�� " << m_clients.size() << "��)\n";

                startRecv(client_port, socket);
            }
            startAccept();
            });
    }

    // ��ε�ĳ��Ʈ ����
    void sendMsgAllClients(std::string msg)
    {
        for (auto& client : m_clients) {
            sendMessage(client.second, msg);
        }
    }

    // �ش� ��Ʈ��ȣ���Ը� �޽��� ����
    void sendOnceClient(std::string msg, unsigned short port)
    {
        auto it = m_clients.find(port);             // Ű������ ã�´� (���� port)
        if (it != m_clients.end()) {
            sendMessage(it->second, msg);
        }
        else {
            printf("�ش� Ŭ���̾�Ʈ�� ã�� �� �����ϴ� [port: %d]\n", port);
        }
    }

    // �޽��� ����
    void sendMessage(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string& msg) {
        auto write_buf = std::make_shared<std::string>(msg);
        boost::asio::async_write(*socket, boost::asio::buffer(*write_buf),
            [socket, write_buf](boost::system::error_code ec, std::size_t) {
                if (ec) {
                    std::cout << "�޽��� ���� ����\n";
                }
            });
    }

    // 
    void startRecv(unsigned short client_port, std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
        auto buffer = std::make_shared<std::vector<char>>(4096);

        socket->async_read_some(boost::asio::buffer(*buffer),
            [this, client_port, socket, buffer](boost::system::error_code ec, std::size_t bytes) {
                if (!ec) {
                    std::string msg(buffer->data(), bytes);
                    std::cout << "[��Ʈ " << client_port << "] ����: " << msg << "\n";

                    // ��ε�ĳ��Ʈ
                    for (auto& [port, sock] : m_clients) {
                        if (port != client_port)
                            sendMessage(sock, msg);
                    }

                    startRecv(client_port, socket);
                }
                else {
                    m_clients.erase(client_port);
                    std::cout << "Ŭ���̾�Ʈ ���� ����: ��Ʈ " << client_port
                        << " (���� �ο�: " << m_clients.size() << "��)\n";
                }
            });
    }

private:
    boost::asio::io_context m_io_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
    unsigned short m_port;
    std::unordered_map<unsigned short, std::shared_ptr<boost::asio::ip::tcp::socket>> m_clients;
};

int main() {
    setlocale(LC_ALL, "");

    printf("����2 �ν�Ʈ ����\n");

    try {
        short port = DEFAULT_PORT;

        CTCPServer server(port);

        std::thread server_thread([&server]() {
            server.startServer();
        });
        server_thread.detach();


        while (true) {
            std::string input = "";
            printf("������ �Է��ϼ���>>");
            std::getline(std::cin, input);
            if (input == "")              // ���� "" �Է� �� ����
            {
                break;
            }
            else 
            {
                server.sendMsgAllClients("��������: " + input);
            }
            
        }

        server.stopServer();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
