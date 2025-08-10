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

            // 수신 메시지를 변수에 저장
            m_recv_msg = std::string(buf, len);
            printf("\n==서버로부터받은 사이즈 :%d==\n", m_recv_msg.size());

            // 변환
            //TCHAR wmsg[10000] = { 0 };
            //int origin_len = (int)strlen(m_recv_msg.c_str());
            //int ilen = MultiByteToWideChar(CP_ACP, 0, m_recv_msg.c_str(), origin_len, NULL, NULL);
            //MultiByteToWideChar(CP_ACP, 0, m_recv_msg.c_str(), origin_len, wmsg, ilen);

            //TestRecvPrint(wmsg);            // 테스트 수신 출력
        }
    }               // receive()

    void send(const std::string& msg)
    {
        std::cout << "서버로 보냄: " << msg << std::endl;
        boost::asio::write(m_socket, boost::asio::buffer(msg));
    }           // send()

    void TestRecvPrint(TCHAR* wmsg)
    {
        _tprintf(_T("서버응답>> %s \n"), wmsg);
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
        printf("입력>>");
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

