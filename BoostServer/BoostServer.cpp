/*
Boost 라이브러리 1.88.0 버전 설치
https://www.boost.org/ -> Download -> boost_1_88_0.zip -> 압축해제
boost_1_88_0 폴더 -> 관리자 권한으로 cmd 실행 -> .\bootstrap.bat 입력하여 실행
.\b2.exe 입력하여 빌드

포함 디렉토리 : C:\boost_1_88_0
라이브러리 디렉토리 : C:\boost_1_88_0\stage\lib
필요한 라이브러리를 입력에 넣어야함

라이브러리 입력(릴리즈64) : libboost_system-vc143-mt-x64-1_88.lib
라이브러리 입력(디버그64) : libboost_system-vc143-mt-gd-x64-1_88.lib
라이브러리 입력(릴리즈32) : libboost_system-vc143-mt-x32-1_88.lib
라이브러리 입력(디버그32) : libboost_system-vc143-mt-gd-x32-1_88.lib
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
        std::cout << "메시지 전송: " << msg << std::endl;

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

                    // 수신 메시지에 대한 후처리는 별도 쓰레드에서 detach 로 진행
                    std::thread([this, msg]() {
                        std::cout << "Received: " << msg << std::endl;
                        for (auto& client : clients_) {
                            if (client != shared_from_this()) {
                                client->deliver(msg);
                            }
                        }
                    }).detach();

                    // 다음 데이터 수신을 위해 read() 재귀적 호출
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
            printf("공지를 입력하세요>>");
            std::getline(std::cin, input);
            if (input == "")              // 빈문자 "" 입력 시 종료
            {
                break;
            }
            else 
            {
                server.sendMsgAllClients("서버공지: " + input);
            }
            
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
