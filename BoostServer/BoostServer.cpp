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

using boost::asio::ip::tcp;
const int DEFAULT_PORT = 5000;

class ClientConnection;
using ClientConnectionPtr = std::shared_ptr<ClientConnection>;

class ClientConnection : public std::enable_shared_from_this<ClientConnection> 
{
public:
    ClientConnection(boost::asio::io_context& _io,
        tcp::socket socket, std::set<ClientConnectionPtr>& clients)
        : m_io(_io)
        , m_socket(std::move(socket))
        , m_clients(clients) 
    {}

    void start() {
        read();
    }

    void deliver(const std::string& msg) {
        std::cout << "�޽��� ����: " << msg << "\n";

        auto write_buf = std::make_shared<std::string>(msg);
        auto self(shared_from_this());

        boost::asio::async_write(
            m_socket
            , boost::asio::buffer(*write_buf)
            , [this, self, write_buf](boost::system::error_code ec, std::size_t) {
                if (ec) {
                    m_clients.erase(self);
                    m_socket.close();
                }
            });
    }

private:
    void read() {
        auto self(shared_from_this());
        auto buffer = std::make_shared<std::vector<char>>(4096);
        m_socket.async_read_some(
            boost::asio::buffer(*buffer),
            [this, self, buffer](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    std::string msg(buffer->data(), bytes_transferred);

                    // ���� �޽����� ���� ��ó���� ���� �����忡�� ����
                    boost::asio::post(m_io, [this, self, msg]() {
                        std::cout << "Received: " << msg << std::endl;
                        for (auto& client : m_clients) {
                            if (client != self) {
                                client->deliver(msg);
                            }
                        }
                    });

                    // ���� ������ ������ ���� read() ����� ȣ��
                    read();
                }
                else {
                    m_clients.erase(shared_from_this());
                    m_socket.close();
                }
            });
    }

    boost::asio::io_context& m_io;
    tcp::socket m_socket;
    std::set<ClientConnectionPtr>& m_clients;
};

class ChatServer {
public:
    ChatServer(short port)
        : m_io_context()
        , m_acceptor(m_io_context, tcp::endpoint(tcp::v4(), port))
    {
        accept();
    }

    virtual ~ChatServer() {
        closeServer();
    }

    void start() {
        m_io_context.run();
    }

    void sendMsgAllClients(std::string msg)
    {
        boost::asio::post(m_io_context, [this, msg]() {
            for (auto& client : m_clients)
            {
                client->deliver(msg);
            }
        });
    }

    void closeServer() {
        m_io_context.stop();
    }

private:
    void accept() {
        m_acceptor.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    auto connection = std::make_shared<ClientConnection>(m_io_context, std::move(socket), m_clients);
                    connection->start();
                    m_clients.insert(connection);
                }

                accept();
            });
    }

    

    boost::asio::io_context m_io_context;
    tcp::acceptor m_acceptor;
    std::set<ClientConnectionPtr> m_clients;
};

int main() {
    setlocale(LC_ALL, "");

    try {
        //boost::asio::io_context io_context;

        short port = DEFAULT_PORT;

        ChatServer server(port);

        std::thread server_thread([&server]() {
            server.start();
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

        server.closeServer();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
