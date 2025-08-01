// Boost 라이브러리 1.83.0 버전
/*
설치
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
                printf("서버 연결 성공: %s:%s\n", m_server.c_str(), m_port.c_str());
                break;
            }
            catch (const std::exception& e)
            {
                printf("서버 연결 실패: %s (%s:%s). 1초 후 재시도합니다...\n", e.what(), m_server.c_str(), m_port.c_str());
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }

    void reconnect()
    {
        boost::system::error_code ec;
        m_socket.close(ec);
        if (ec) { printf("소켓 close 실패: %s\n", ec.message().c_str()); }
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

                // 수신 메시지를 변수에 저장
                m_recv_msg = std::string(buf, len);
                printf("\n==서버로부터받은것 :%s==\n", m_recv_msg.c_str());

                // 변환
                TCHAR wmsg[1024] = { 0 };
                int origin_len = (int)strlen(m_recv_msg.c_str());
                int ilen = MultiByteToWideChar(CP_ACP, 0, m_recv_msg.c_str(), origin_len, NULL, NULL);
                MultiByteToWideChar(CP_ACP, 0, m_recv_msg.c_str(), origin_len, wmsg, ilen);

                TestRecvPrint(wmsg);            // 테스트 수신 출력
            }
            catch (const boost::system::system_error& e)
            {
                printf("소켓 에러 발생: %s. 1초 후 재연결...\n", e.what());
                std::this_thread::sleep_for(std::chrono::seconds(1));
                reconnect();
            }
            catch (const std::exception& e)
            {
                printf("예외 발생: %s. 1초 후 재연결...\n", e.what());
                std::this_thread::sleep_for(std::chrono::seconds(1));
                reconnect();
            }
            catch (...)
            {
                printf("알 수 없는 예외 발생, 1초 후 재연결...\n");
                std::this_thread::sleep_for(std::chrono::seconds(1));
                reconnect();
            }
        }
    }               // receive()

    void send(const std::string& msg)
    {
        try {
            std::cout << "서버로 보냄: " << msg << std::endl;
            boost::asio::write(m_socket, boost::asio::buffer(msg));
        }
        catch (const std::exception& e) {
            printf("Send 실패: %s. 재연결 시도합니다...\n", e.what());
            reconnect();
            // 재시도용 로직: 필요하면 send(msg) 재귀 호출하거나 큐에 저장 후 재전송
        }
    }           // send()

    void TestRecvPrint(TCHAR* wmsg)
    {
        _tprintf(_T("서버응답>> %s \n"), wmsg);
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
        printf("입력>>");
        rewind(stdin);
        memset(msg, 0, sizeof(msg));
        memset(w_msg, 0, sizeof(w_msg));
        _tscanf_s(_T("%[^\n]s"), w_msg, 1024);
        int len = WideCharToMultiByte(CP_ACP, 0, w_msg, -1, NULL, 0, NULL, NULL);
        WideCharToMultiByte(CP_ACP, 0, w_msg, -1, msg, len, NULL, NULL);

        client.send(msg);
    }
}

