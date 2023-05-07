#include <cstdio>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <tchar.h>

using boost::asio::ip::tcp;

// Boost 라이브러리 1.82.0 버전
/*
설치
https://www.boost.org/ -> Download -> boost_1_82_0.zip -> 압축해제
boost_1_82_0 폴더 -> 관리자 권한으로 cmd 실행 -> .\bootstrap.bat 입력하여 실행
.\b2.exe 입력하여 빌드

포함 디렉토리 : boost_1_82_0
라이브러리 디렉토리 : boost_1_82_0\stage\lib
필요한 라이브러리를 입력에 넣어야함

라이브러리 입력(릴리즈64) : libboost_system-vc143-mt-x64-1_82.lib
라이브러리 입력(디버그64) : libboost_system-vc143-mt-gd-x64-1_82.lib
라이브러리 입력(릴리즈32) : libboost_system-vc143-mt-x32-1_82.lib
라이브러리 입력(디버그32) : libboost_system-vc143-mt-gd-x32-1_82.lib
*/

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
            char buf[1024];
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

            // 변환
            int origin_len = strlen(m_recv_msg.c_str());
            int ilen = MultiByteToWideChar(CP_ACP, 0, m_recv_msg.c_str(), origin_len, NULL, NULL);
            MultiByteToWideChar(CP_ACP, 0, m_recv_msg.c_str(), origin_len, m_wmsg, ilen);

            TestRecvPrint(m_wmsg);            // 테스트 수신 출력
        }
    }               // receive()

    void send(const std::string& msg)
    {
        boost::asio::write(m_socket, boost::asio::buffer(msg));
    }           // send()

    void TestRecvPrint(TCHAR* wmsg)
    {
        _tprintf(_T("서버응답>>"), wmsg);
    }           // TestRecvPrint()

private:
    boost::asio::io_context io_context;
    tcp::socket m_socket;
    std::string m_recv_msg;
    TCHAR m_wmsg[1024];
};

int main()
{
    _tsetlocale(0, _T("korean"));

    std::string server = "127.0.0.1";
    std::string port = "5000";

    SocketClient client(server, port);

    std::thread receive_thread(&SocketClient::receive, &client);

    TCHAR w_msg[1024];
    char msg[1024];

    while (1) {
        printf("입력>>");
        _tscanf(_T("%s"), msg);
        int len = WideCharToMultiByte(CP_ACP, 0, w_msg, -1, NULL, 0, NULL, NULL);
        WideCharToMultiByte(CP_ACP, 0, w_msg, -1, msg, len, NULL, NULL);

        client.send(msg);
    }

    receive_thread.join();
}

