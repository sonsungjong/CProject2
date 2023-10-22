#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <iphlpapi.h>
#include <IcmpAPI.h>
#include <thread>
#include <chrono>
#include <future>
#include <string>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

// 12��
struct PingResponse
{
    bool pc1 = false;
    bool pc2 = false;
    bool pc3 = false;
    bool pc4 = false;
    bool pc5 = false;
    bool pc6 = false;
    bool pc7 = false;
    bool pc8 = false;
    bool pc9 = false;
    bool pc10 = false;
    bool pc11 = false;
    bool pc12 = false;
};

class Pinger
{
private:
    const std::string ips[12] = { 
        "172.16.1.156","172.16.1.157", "172.16.1.158","172.16.1.159",
        "172.16.1.160","172.16.1.161","172.16.1.162","172.16.1.163",
        "172.16.1.164", "172.16.1.165","172.16.1.166","172.16.1.167"
    };           // 12�� ��ǻ���� IP
    DWORD dw_timeout = 500;             // 500ms Ÿ�Ӿƿ�
    HANDLE h_icmp_file;

public:
    Pinger() {
        h_icmp_file = IcmpCreateFile();
        if (h_icmp_file == INVALID_HANDLE_VALUE) {
            printf("IcmpCreateFile Failed.\n");
            exit(1);
        }
    }

    virtual ~Pinger() {
        IcmpCloseHandle(h_icmp_file);
    }

    // ��ǻ�Ϳ� ���� �����Ͽ� true/false�� ��ȯ�ϴ� �Լ�
    bool SendPing(const std::string& a_address) {
        DWORD dw_ret_val;
        const int buffer_size = sizeof(ICMP_ECHO_REPLY) + 32 +8;
        BYTE reply_buffer[buffer_size];
        DWORD dw_reply_size = sizeof(reply_buffer);
        bool ping_result = false;

        dw_ret_val = IcmpSendEcho(h_icmp_file, inet_addr(a_address.c_str()), NULL, 0, NULL, reply_buffer, dw_reply_size, dw_timeout);

        ping_result = dw_ret_val != 0;          // �� ����� true/false�� ��ȯ
        return ping_result;
    }

    // SendPing�Լ��� �񵿱�� �����Ű�� �Լ�
    std::future<bool> SendPingAsync(const std::string& a_address)
    {
        return std::async(std::launch::async, [this, a_address]() {return this->SendPing(a_address); });        // SendPing
    }

    // �� ��ǻ�͸��� �γ����⸦ �񵿱�� ������� ����ü�� ���¸� ������Ʈ�ϴ� �Լ�
    void CheckPings(PingResponse& response) {
        while (true) {
            auto pc1_future = SendPingAsync(ips[0]);
            auto pc2_future = SendPingAsync(ips[1]);
            auto pc3_future = SendPingAsync(ips[2]);
            auto pc4_future = SendPingAsync(ips[3]);
            auto pc5_future = SendPingAsync(ips[4]);
            auto pc6_future = SendPingAsync(ips[5]);
            auto pc7_future = SendPingAsync(ips[6]);
            auto pc8_future = SendPingAsync(ips[7]);
            auto pc9_future = SendPingAsync(ips[8]);
            auto pc10_future = SendPingAsync(ips[9]);
            auto pc11_future = SendPingAsync(ips[10]);
            auto pc12_future = SendPingAsync(ips[11]);

            response.pc1 = pc1_future.get();
            response.pc2 = pc2_future.get();
            response.pc3 = pc3_future.get();
            response.pc4 = pc4_future.get();
            response.pc5 = pc5_future.get();
            response.pc6 = pc6_future.get();
            response.pc7 = pc7_future.get();
            response.pc8 = pc8_future.get();
            response.pc9 = pc9_future.get();
            response.pc10 = pc10_future.get();
            response.pc11 = pc11_future.get();
            response.pc12 = pc12_future.get();

            printf("Ping���� ���� %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 
                response.pc1, response.pc2, response.pc3, response.pc4, 
                response.pc5, response.pc6, response.pc7, response.pc8, 
                response.pc9, response.pc10, response.pc11, response.pc12);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));       // 0.5�� + ���ð� (0~0.5��) ����
        }

    }
};

int main()
{
    Pinger pinger;                              // ���� �����ϴ� Ŭ����
    PingResponse response;              // �� ���¸� ������ ����ü
    
    // ������ �����忡�� CheckPings�Լ��� ����
    std::thread ping_thread([&]() {pinger.CheckPings(response); });                  // �� ��ǻ�Ϳ� ���� ������ ������ �޾� ����ü�� ������

    while (true) {
        printf("���ξ����� Ping���� Ȯ�� %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
            response.pc1, response.pc2, response.pc3, response.pc4,
            response.pc5, response.pc6, response.pc7, response.pc8,
            response.pc9, response.pc10, response.pc11, response.pc12);

        std::this_thread::sleep_for(std::chrono::seconds(2));               // ���ξ����忡�� 2�ʸ��� ����ü ���� üũ
    }
}

