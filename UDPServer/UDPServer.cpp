#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdio>
#include <tchar.h>
#pragma comment(lib, "WS2_32.lib")

#define PORT				23456
#define BASIC_IP			_T("127.0.0.1")

void IPCatch(char* my_ip)
{
	WORD wVersionRequested = MAKEWORD(0x02, 0x02);
	WSADATA wsaData;

	//char my_ip[256];
	//ZeroMemory(my_ip, 256);

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
	//printf("%s", my_ip);
}

int main()
{
	WSADATA data;
	WSAStartup(0x0202, &data);

	char serverIP[256] = { 0 };
	IPCatch(serverIP);

	SOCKET listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addr_data = { AF_INET, htons(PORT), };
	InetPtonA(AF_INET, serverIP, &addr_data.sin_addr.s_addr);		// 내 PC 아이피
	//InetPton(AF_INET, BASIC_IP, &addr_data.sin_addr.s_addr);			// 127.0.0.1

	bind(listen_socket, (sockaddr*)&addr_data, sizeof(addr_data));

	sockaddr_in recv_socket;
	ZeroMemory(&recv_socket, sizeof(recv_socket));
	int recv_len = sizeof(recv_socket);

	char buf[1024] = { 0 };
	char clientIP[256];


	printf("서버의 아이피 : %s\n", serverIP);

	while (true)
	{
		ZeroMemory(buf, 1024);


		int byesIn = recvfrom(listen_socket, buf, 1024, 0, (sockaddr*)&recv_socket, &recv_len);

		ZeroMemory(clientIP, 256);
		inet_ntop(AF_INET, &recv_socket.sin_addr, clientIP, 256);
		printf("Message rect from %s : %s \n", clientIP, buf);

		// 로그파일 찍기
		FILE* p_file;
		p_file = fopen("C:\\log\\udp_server_tester.txt", "at");
		if (NULL != p_file) {
			fprintf(p_file, buf);
			fprintf(p_file, "\n");
			fclose(p_file);	
		}

	}

	closesocket(listen_socket);

	WSACleanup();

	return 0;
}