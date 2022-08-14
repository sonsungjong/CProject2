#include <cstdio>
#include <WS2tcpip.h>
#include <string>
#include <tchar.h>
#pragma comment(lib, "WS2_32.lib")

#define PORT 40004
#define IP _T("61.84.89.106")

int main()
{
	WSADATA data;
	WSAStartup(0x0202, &data);

	SOCKET listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addr_data = { AF_INET, htons(PORT), };
	InetPton(AF_INET, IP, &addr_data.sin_addr.s_addr);

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	InetPton(AF_INET, IP, &server.sin_addr);

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	char ss[256];

	while(true){
		printf("보낼 메시지 >>");
		scanf_s("%s", &ss, 256);
		sendto(out, ss, 256, 0, (sockaddr*)&server, sizeof(server));
		if (0 == strcmp(ss, "hello"))
		{
			break;			// 입력한 메시지가 hello면 종료
		}
	}
	closesocket(listen_socket);

	WSACleanup();
}
