#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <WS2tcpip.h>
#include <string>
#include <tchar.h>
#pragma comment(lib, "WS2_32.lib")

#define MY_PORT			33033
#define REMOTE_PORT 23456
#define REMOTE_IP			_T("127.0.0.1")

// 내 아이피를 얻는다
void IPCatch(char* my_ip)
{
	WORD wVersionRequested = MAKEWORD(0x02, 0x02);
	WSADATA wsaData;

	//char my_ip[256];
	//ZeroMemory(my_ip, 256);

	if (0 == WSAStartup(wVersionRequested, &wsaData))
	{
		char ucHostName[255];
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
	//printf("%s", my_ip);
}

int main()
{
	WSADATA data;
	(void)WSAStartup(0x0202, &data);

	char my_ip[256] = { 0 };
	IPCatch(my_ip);

	SOCKET listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addr_data = { AF_INET, htons(MY_PORT), };
	InetPtonA(AF_INET, my_ip, &addr_data.sin_addr.s_addr);

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(REMOTE_PORT);
	//InetPton(AF_INET, REMOTE_IP, &server.sin_addr);				// 127.0.0.1
	InetPtonA(AF_INET, my_ip, &server.sin_addr);					// 같은 PC일때만(내 PC 아이피)

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	std::string my_msg = "";

	while(true){
		my_msg.clear();
		printf("보낼 메시지 >>");
		std::getline(std::cin, my_msg);
		sendto(out, my_msg.c_str(), my_msg.size()+1, 0, (sockaddr*)&server, sizeof(server));
		if (0 == strcmp(my_msg.c_str(), "0"))
		{
			break;			// 입력한 메시지가 0면 종료
		}
	}
	closesocket(listen_socket);

	WSACleanup();
}
