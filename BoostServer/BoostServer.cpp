/*
Boost 라이브러리 1.83.0 버전 설치
https://www.boost.org/ -> Download -> boost_1_83_0.zip -> 압축해제
boost_1_83_0 폴더 -> 관리자 권한으로 cmd 실행 -> .\bootstrap.bat 입력하여 실행
.\b2.exe 입력하여 빌드

포함 디렉토리 : C:\boost_1_83_0
라이브러리 디렉토리 : C:\boost_1_83_0\stage\lib
필요한 라이브러리를 입력에 넣어야함

라이브러리 입력(릴리즈64) : libboost_system-vc142-mt-x64-1_83.lib
라이브러리 입력(디버그64) : libboost_system-vc142-mt-gd-x64-1_83.lib
라이브러리 입력(릴리즈32) : libboost_system-vc142-mt-x32-1_83.lib
라이브러리 입력(디버그32) : libboost_system-vc142-mt-gd-x32-1_83.lib
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
        std::cout << "메시지 전송: " << msg << "\n";

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

                    // 수신 메시지에 대한 후처리는 별도 쓰레드에서 진행
                    boost::asio::post(m_io, [this, self, msg]() {
                        std::cout << "Received: " << msg << std::endl;
                        for (auto& client : m_clients) {
                            if (client != self) {
                                client->deliver(msg);
                            }
                        }
                    });

                    // 다음 데이터 수신을 위해 read() 재귀적 호출
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

        server.closeServer();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
