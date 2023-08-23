#include <Windows.h>
#include <stdio.h>
#include <string>

#pragma comment(lib, "WS2_32.lib")

std::string CheckIP()
{
	char my_ip[256] = { 0, };
	std::string ip = "";
	WORD wVersionRequested = MAKEWORD(0x02, 0x02);
	WSADATA wsaData;

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
					ip = std::string(my_ip);
				}
			}
		}
	}
	WSACleanup();
	return ip;
}

void main()
{
	std::string ip = CheckIP();
	printf("%s\n", ip.c_str());
}