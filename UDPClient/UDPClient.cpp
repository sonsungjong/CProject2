#include <cstdio>
#include <WS2tcpip.h>
#include <string>
#include <tchar.h>
#pragma comment(lib, "WS2_32.lib")

#define PORT 23456
#define IP _T("172.16.1.157")

int main()
{
	WSADATA data;
	(void)WSAStartup(0x0202, &data);

	SOCKET listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addr_data = { AF_INET, htons(PORT), };
	InetPton(AF_INET, IP, &addr_data.sin_addr.s_addr);

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	InetPton(AF_INET, IP, &server.sin_addr);

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	char my_msg[256] = { 0, };

	while(true){
		printf("보낼 메시지 >>");
		scanf_s("%s", &my_msg, 256);
		sendto(out, my_msg, 256, 0, (sockaddr*)&server, sizeof(server));
		if (0 == strcmp(my_msg, "hello"))
		{
			break;			// 입력한 메시지가 hello면 종료
		}
	}
	closesocket(listen_socket);

	WSACleanup();
}
