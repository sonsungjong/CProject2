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

    // TCP 서버 시작
    void startServer() {
        m_io_context.run();
    }

    // TCP 서버 종료
    void stopServer() {
        m_io_context.stop();
    }

    // 클라이언트 접속포트 생성
    void startAccept() {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_io_context);
        m_acceptor.async_accept(*socket, [this, socket](boost::system::error_code ec) {
            if (!ec) {
                auto endpoint = socket->remote_endpoint();
                unsigned short client_port = endpoint.port();
                m_clients[client_port] = socket;

                std::cout << "클라이언트 접속: 포트 " << client_port << " (총 " << m_clients.size() << "명)\n";

                startRecv(client_port, socket);
            }
            startAccept();
            });
    }

    // 브로드캐스트 전송
    void sendMsgAllClients(std::string msg)
    {
        for (auto& client : m_clients) {
            sendMessage(client.second, msg);
        }
    }

    // 해당 포트번호에게만 메시지 전송
    void sendOnceClient(std::string msg, unsigned short port)
    {
        auto it = m_clients.find(port);             // 키값으로 찾는다 (현재 port)
        if (it != m_clients.end()) {
            sendMessage(it->second, msg);
        }
        else {
            printf("해당 클라이언트를 찾을 수 없습니다 [port: %d]\n", port);
        }
    }

    // 메시지 전송
    void sendMessage(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string& msg) {
        auto write_buf = std::make_shared<std::string>(msg);
        boost::asio::async_write(*socket, boost::asio::buffer(*write_buf),
            [socket, write_buf](boost::system::error_code ec, std::size_t) {
                if (ec) {
                    std::cout << "메시지 전송 실패\n";
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
                    std::cout << "[포트 " << client_port << "] 수신: " << msg << "\n";

                    // 브로드캐스트
                    for (auto& [port, sock] : m_clients) {
                        if (port != client_port)
                            sendMessage(sock, msg);
                    }

                    startRecv(client_port, socket);
                }
                else {
                    m_clients.erase(client_port);
                    std::cout << "클라이언트 연결 종료: 포트 " << client_port
                        << " (남은 인원: " << m_clients.size() << "명)\n";
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

    printf("버전2 부스트 서버\n");

    try {
        short port = DEFAULT_PORT;

        CTCPServer server(port);

        std::thread server_thread([&server]() {
            server.startServer();
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

        server.stopServer();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
