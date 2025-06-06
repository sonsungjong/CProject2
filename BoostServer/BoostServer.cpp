/*
Boost ���̺귯�� 1.88.0 ���� ��ġ
https://www.boost.org/ -> Download -> boost_1_88_0.zip -> ��������
boost_1_88_0 ���� -> ������ �������� cmd ���� -> .\bootstrap.bat �Է��Ͽ� ����
.\b2.exe �Է��Ͽ� ����

���� ���丮 : C:\boost_1_88_0
���̺귯�� ���丮 : C:\boost_1_88_0\stage\lib
�ʿ��� ���̺귯���� �Է¿� �־����

���̺귯�� �Է�(������64) : libboost_system-vc143-mt-x64-1_88.lib
���̺귯�� �Է�(�����64) : libboost_system-vc143-mt-gd-x64-1_88.lib
���̺귯�� �Է�(������32) : libboost_system-vc143-mt-x32-1_88.lib
���̺귯�� �Է�(�����32) : libboost_system-vc143-mt-gd-x32-1_88.lib
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
#pragma comment(lib, "libboost_system-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "libboost_locale-vc143-mt-gd-x64-1_88.lib")
#else
#pragma comment(lib, "libboost_system-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "libboost_locale-vc143-mt-x64-1_88.lib")
#endif

using boost::asio::ip::tcp;
const int DEFAULT_PORT = 5000;

class ClientConnection;
using ClientConnectionPtr = std::shared_ptr<ClientConnection>;

class ClientConnection : public std::enable_shared_from_this<ClientConnection> 
{
public:
    ClientConnection(tcp::socket socket, std::set<ClientConnectionPtr>& clients)
        : socket_(std::move(socket)), clients_(clients) {}

    void start() {
        read();
    }

    void deliver(const std::string& msg) {
        std::cout << "�޽��� ����: " << msg << std::endl;

        auto self(shared_from_this());
        boost::asio::async_write(
            socket_, boost::asio::buffer(msg),
            [this, self](boost::system::error_code ec, std::size_t) {
                if (ec) {
                    clients_.erase(shared_from_this());
                }
            });
    }

private:
    void read() {
        auto self(shared_from_this());
        memset(buffer_, 0, sizeof(buffer_));
        socket_.async_read_some(
            boost::asio::buffer(buffer_),
            [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    std::string msg(buffer_, bytes_transferred);

                    // ���� �޽����� ���� ��ó���� ���� �����忡�� detach �� ����
                    std::thread([this, msg]() {
                        std::cout << "Received: " << msg << std::endl;
                        for (auto& client : clients_) {
                            if (client != shared_from_this()) {
                                client->deliver(msg);
                            }
                        }
                    }).detach();

                    // ���� ������ ������ ���� read() ����� ȣ��
                    read();
                }
                else {
                    clients_.erase(shared_from_this());
                }
            });
    }

    tcp::socket socket_;
    std::set<ClientConnectionPtr>& clients_;
    char buffer_[1024] = { 0, };
};

class ChatServer {
public:
    ChatServer(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        accept();
    }

    void sendMsgAllClients(std::string msg)
    {
        std::thread([this, msg]() {
            for (auto& client : clients_)
            {
                client->deliver(msg);
            }
        }).detach();
    }

private:
    void accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    auto connection = std::make_shared<ClientConnection>(std::move(socket), clients_);
                    connection->start();
                    clients_.insert(connection);
                }

                accept();
            });
    }

    tcp::acceptor acceptor_;
    std::set<ClientConnectionPtr> clients_;
};

int main() {
    try {
        boost::asio::io_context io_context;

        short port = DEFAULT_PORT;

        ChatServer server(io_context, port);

        std::thread server_thread([&io_context]() {
            io_context.run();
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
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
