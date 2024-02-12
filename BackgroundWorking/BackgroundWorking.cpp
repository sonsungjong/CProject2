#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <chrono>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

#define DST_IP              "127.0.0.1"
#define THIS_PORT         8888
#define DST_PORT            28888

// 1. 메인 UI쓰레드 2. 입출력 쓰레드
// 입출력 쓰레드가 메시지나 이벤트를 UI쓰레드에게 보내어 UI를 업데이트
class BackgroundWorking {
private:
    std::mutex& m_mtx;
    std::string& m_recvString;
    std::string& m_keyboardInput;
    SOCKET m_listen_socket;
    sockaddr_in m_listen_addr;

public:
    BackgroundWorking(std::mutex& mtx, std::string& ui_msg, std::string& input)
        : m_recvString(ui_msg), m_mtx(mtx), m_keyboardInput(input)
    {
        WSADATA wsa_data;
        (void)WSAStartup(0x0202, &wsa_data);
        m_listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
        m_listen_addr.sin_family = AF_INET;
        m_listen_addr.sin_addr.s_addr = INADDR_ANY;
        m_listen_addr.sin_port = htons(THIS_PORT);
        (void)bind(m_listen_socket, reinterpret_cast<const struct sockaddr*>(&m_listen_addr), sizeof(m_listen_addr));
    }

    virtual ~BackgroundWorking()
    {
        closesocket(m_listen_socket);
        WSACleanup();
    }

    // 수신메시지 UI에 반영
    void recvData() {
        std::unique_ptr<char[]> p_buffer(new char[65535]);
        sockaddr_in dst_addr;
        int len = sizeof(dst_addr);

        while (true) 
        {
            memset(p_buffer.get(), 0, 65535);
            int recv_len = recvfrom(m_listen_socket, p_buffer.get(), 65535 - 1, 0, reinterpret_cast<struct sockaddr*>(&dst_addr), &len);

            if (recv_len > 0) {
                std::lock_guard<std::mutex> lock(m_mtx);
                m_recvString = p_buffer.get();
            }
        }
    }

    // 입력받은 것 전송
    void sendData()
    {
        sockaddr_in dst_addr = { 0, };
        dst_addr.sin_family = AF_INET;
        dst_addr.sin_port = htons(DST_PORT);
        inet_pton(AF_INET, DST_IP, &dst_addr.sin_addr);
        {
            std::lock_guard<std::mutex> lock(m_mtx);
            sendto(m_listen_socket, m_keyboardInput.c_str(), m_keyboardInput.length(), 0, (struct sockaddr*)(&dst_addr), sizeof(dst_addr));
        }
    }

    void start() {
        std::thread backgroundThread(&BackgroundWorking::recvData, this);
        backgroundThread.detach();
    }
};

class KeyboardClass
{
public:
    KeyboardClass(std::mutex& mtx, std::string& key, BackgroundWorking& network)
        : m_inputString(key), m_mtx(mtx), m_network(network)
    {}

    void inputHandler() {
        std::string keyboardInput;
        while (true) {
            std::getline(std::cin, keyboardInput);
            {
                {
                    std::lock_guard<std::mutex> lock(m_mtx);
                    m_inputString = keyboardInput;
                }
                m_network.sendData();
            }
        }
    }

    void start()
    {
        std::thread inputThread(&KeyboardClass::inputHandler, this);
        inputThread.detach();
    }
private:
    std::string& m_inputString;
    std::mutex& m_mtx;
    BackgroundWorking& m_network;
};

int main()
{
    std::string str_recv = "Hello World!";
    std::string str_input = "123";
    std::mutex mtx;             // 공유데이터에 대한 접근을 동기화
    BackgroundWorking back(mtx, str_recv, str_input);
    KeyboardClass keyboard(mtx, str_input, back);
    back.start();
    keyboard.start();

     //메인 UI쓰레드
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            printf("==%s==\n", str_recv.c_str());
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
