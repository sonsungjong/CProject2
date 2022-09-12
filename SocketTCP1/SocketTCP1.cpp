#include "pch.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "framework.h"
#include "SocketTCP1.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

#define IP					_T("222.100.255.148")
#define PORT				19000

// Ŭ���̾�Ʈ ����ó���� ���� ����
SOCKET gh_listen_socket = INVALID_SOCKET;
// Ŭ���̾�Ʈ �޽����� �����ϱ� ���� ����
SOCKET gh_client_socket = INVALID_SOCKET;

void StartListenService(const TCHAR* ap_ip_address, INT a_port, HWND ah_wnd)
{
	WSADATA temp;
	(void)WSAStartup(0x0202, &temp);			// ���� ���̺귯�� ��밡�ɻ��� ON

	gh_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr_data = { AF_INET, htons(a_port), };
	InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);

	bind(gh_listen_socket, (sockaddr*)&addr_data, sizeof(addr_data));
	listen(gh_listen_socket, 5);

	WSAAsyncSelect(gh_listen_socket, ah_wnd, 25001, FD_ACCEPT);
}

void ProcessAccept(HWND ah_wnd)
{
	sockaddr_in addr_data;
	INT addr_data_size = sizeof(addr_data);

	SOCKET h_socket = accept(gh_listen_socket, (sockaddr*)&addr_data, &addr_data_size);

	if (gh_client_socket == INVALID_SOCKET)
	{
		gh_client_socket = h_socket;

		INT temp_size = 200000;
		INT read_size = 4;
		setsockopt(gh_client_socket, SOL_SOCKET, SO_SNDBUF, (char*)&temp_size, 4);
		setsockopt(gh_client_socket, SOL_SOCKET, SO_RCVBUF, (char*)&temp_size, 4);

		// �����ϸ� 25002�� �߻����Ѷ�
		WSAAsyncSelect(gh_client_socket, ah_wnd, 25002, FD_READ | FD_CLOSE);
		
		// ���� �޽��� ������
		char data[10004] = { 0, };
		*(int*)data = 10000;
		send(gh_client_socket, data, 10004, 0);
	}
	else {
		closesocket(h_socket);
	}
}

// recv�Լ� ���� -> ���� �������� ��� ��õ��ϰԲ� ����
INT ReceiveData(SOCKET ah_socket, CHAR* ap_data, INT a_size)
{
	INT total_size = 0;
	INT read_size = 0;
	INT retry_count = 0;
	while (total_size < a_size)
	{
		read_size = recv(ah_socket, ap_data + total_size, a_size - total_size, 0);
		if (read_size == SOCKET_ERROR || read_size == 0)
		{
			Sleep(10);
			retry_count++;
			if (retry_count > 300) {
				break;
			}
		}
		else {
			retry_count = 0;
			total_size += read_size;			// ���ŵ� ũ�⸦ �ջ�
		}
	}
	return total_size;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY)
	{
		closesocket(gh_listen_socket);
		if (gh_client_socket != INVALID_SOCKET) {
			closesocket(gh_client_socket);
		}
		WSACleanup();
		PostQuitMessage(0);
	}
	else if (msg == WM_CREATE){
		StartListenService(IP, PORT, hWnd);								// �������� ����
	}
	else if (msg == 25001) {				// Ŭ���̾�Ʈ ����
		ProcessAccept(hWnd);
	}
	else if (msg == 25002) {						// ������ ���� �Ǵ� Ŭ���̾�Ʈ ���� ����
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {
			// ������ ����
			int data;
			// recv(gh_client_socket, (char*)&data, 4, 0);					// 4����Ʈ ũ�⸦, data�� ����
			ReceiveData(gh_client_socket, (char*)&data, 4);
		}
		else {				// Ŭ���̾�Ʈ�� ���� ����
			closesocket(gh_client_socket);
			gh_client_socket = INVALID_SOCKET;
		}
	}

	// �����ڰ� ó������ ���� �޽������� ó��
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow)
{
	WNDCLASS wc;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = _T("SockServer123");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	HWND hWnd = CreateWindow(_T("SockServer123"), _T("Win32 Server Socket"), WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<INT>(msg.wParam);
}

