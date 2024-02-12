#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <WS2tcpip.h>
#include <string>
#include <tchar.h>
#include <thread>
#include <future>
#include <chrono>
#pragma comment(lib, "WS2_32.lib")

#define THIS_PORT			28888
#define DST_PORT            8888
#define DST_IP					"127.0.0.1"

class UDPUnicast
{
public:

    void IPCatch(char* my_ip)
    {
		WORD wVersionRequested = MAKEWORD(0x02, 0x02);
		WSADATA wsaData;

		if (0 == WSAStartup(wVersionRequested, &wsaData))
		{
			char ucHostName[255] = { 0 };
			if (0 == gethostname(ucHostName, sizeof(ucHostName)))
			{
				PHOSTENT phostInfo = gethostbyname(ucHostName);
				if (NULL != phostInfo)
				{
					for (int i = 0; phostInfo->h_addr_list[i] != NULL; i++)
					{
						int iIP[4] = { 0, };
						in_addr* pAddr = (in_addr*)phostInfo->h_addr_list[i];
						iIP[0] = pAddr->S_un.S_un_b.s_b1;
						iIP[1] = pAddr->S_un.S_un_b.s_b2;
						iIP[2] = pAddr->S_un.S_un_b.s_b3;
						iIP[3] = pAddr->S_un.S_un_b.s_b4;
						sprintf_s(my_ip, 256, "%d.%d.%d.%d", iIP[0], iIP[1], iIP[2], iIP[3]);
					}
				}
			}
		}
		printf("%s", my_ip);
    }

	// 생성자
	UDPUnicast(unsigned short a_my_port)
	{
		memset(m_my_ip, 0, 256);
		IPCatch(m_my_ip);
		char target_ip[256] = { 0, };
		memcpy(target_ip, m_my_ip, 256);
		m_listen_socket = INVALID_SOCKET;

		WSADATA wsa_data;
		int result = WSAStartup(0x0202, &wsa_data);
		if (result != 0) {
			std::cerr << "WSAStartup Failed: " << result << std::endl;
			exit(1);
		}

		// 소켓 생성
		m_listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
		if (m_listen_socket == INVALID_SOCKET) {
			std::cerr << "Error opening socket: " << WSAGetLastError() << std::endl;
			WSACleanup();
			exit(1);
		}
		m_listen_addr.sin_family = AF_INET;
		m_listen_addr.sin_addr.s_addr = INADDR_ANY;
		m_listen_addr.sin_port = htons(a_my_port);

		if (bind(m_listen_socket, reinterpret_cast<const struct sockaddr*>(&m_listen_addr), sizeof(m_listen_addr)) == SOCKET_ERROR)
		{
			std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
			closesocket(m_listen_socket);
			WSACleanup();
			exit(1);
		}

		// 별도의 쓰레드를 생성하여 무한루프로 수신을 받음
		m_recv_thread = std::thread(&UDPUnicast::recvData, this);

	}

	void sendData(const std::string& a_msg, const std::string &a_address, int a_port)
	{
		sockaddr_in dst_addr = { 0, };

		dst_addr.sin_family = AF_INET;
		dst_addr.sin_port = htons(a_port);
		inet_pton(AF_INET, a_address.c_str(), &dst_addr.sin_addr);
		sendto(m_listen_socket, a_msg.c_str(), static_cast<int>(a_msg.length()), 0, reinterpret_cast<struct sockaddr*>(&dst_addr), sizeof(dst_addr));
		printf("[send]%s\n", a_msg.c_str());
	}

	void recvData()
	{
		printf("수신 시작\n");
		std::unique_ptr<char[]> p_buffer(new char[65535]);
		sockaddr_in dst_addr;
		int len = sizeof(dst_addr);

		while (true) {
			memset(p_buffer.get(), 0, 65535);
			int n = recvfrom(m_listen_socket, p_buffer.get(), 65535 - 1, 0, reinterpret_cast<struct sockaddr*>(&dst_addr), &len);
			if (n == SOCKET_ERROR) {
				std::cerr << "recvfrom failed with error: " << WSAGetLastError() << std::endl;
				continue;
			}

			p_buffer.get()[n] = '\0';
			printf("[From IP: %s, Port: %d]%s\n", inet_ntoa(dst_addr.sin_addr), ntohs(dst_addr.sin_port), p_buffer.get());
		}
	}

	virtual ~UDPUnicast()
	{
		closesocket(m_listen_socket);

		WSACleanup();
		if (m_recv_thread.joinable()) {
			m_recv_thread.join();
		}
	}

	char m_my_ip[256] = { 0 };
private:
	SOCKET m_listen_socket;
	sockaddr_in m_listen_addr;
	std::thread m_recv_thread;
};

int main()
{
	UDPUnicast udp1(THIS_PORT);

	while (true) {
		std::string msg = "";
		printf("메시지를 입력하세요:");
		std::getline(std::cin, msg);
		udp1.sendData(msg, DST_IP, DST_PORT);				// 호출할 때마다 UDP메시지 송신
	}
}
