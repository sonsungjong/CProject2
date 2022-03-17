// Win32SockClient.cpp : Defines the entry point for the application.
//

#include "pch.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "framework.h"
#include "Win32SockClient.h"

#define MAX_SEND_DATA_SIZE              1000000
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

// ������ ����� ���� ����
SOCKET gh_socket = INVALID_SOCKET;

// ������ ������ �����͸� ������ �� ����ϴ� �Լ�
int ReceiveData(char* ap_data, int a_size)
{
	int total_size = 0, read_size, retry_count = 0;
	while (total_size < a_size) {
		read_size < recv(gh_socket, ap_data + total_size, a_size - total_size, 0);
		if (read_size == SOCKET_ERROR || read_size == 0) {
			Sleep(10);
			retry_count++;
			if (retry_count > 300) { break; }
		}
		else {
			retry_count = 0;
			total_size += read_size;
		}
	}
	return total_size;
}

// ������ ������ �õ��ϴ� �Լ�
void ConnectToServer(const TCHAR* ap_ip_address, int a_port, HWND ah_wnd)
{
	WSADATA temp;
	WSAStartup(0x0202, &temp);

	gh_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr_data = { AF_INET, htons(a_port), };
	InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);
	WSAAsyncSelect(gh_socket, ah_wnd, 26001, FD_CONNECT);
	connect(gh_socket, (sockaddr*)&addr_data, sizeof(addr_data));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) {
		if (gh_socket != INVALID_SOCKET) { closesocket(gh_socket); }
		PostQuitMessage(0);					// WM_QUIT �޽����� �޽���ť�� ����
	}
	else if (uMsg == WM_CREATE) {
		ConnectToServer(IP, PORT, hWnd);
	}
	else if (uMsg == 26001) {
		if (WSAGETSELECTERROR(lParam)) {
			closesocket(gh_socket);
			gh_socket = INVALID_SOCKET;
		}
		else {
			int temp_size = 200000, read_size = 4;
			setsockopt(gh_socket, SOL_SOCKET, SO_SNDBUF, (char*)&temp_size, 4);
			setsockopt(gh_socket, SOL_SOCKET, SO_RCVBUF, (char*)&temp_size, 4);
			WSAAsyncSelect(gh_socket, hWnd, 26002, FD_READ | FD_CLOSE);
		}
	}
	else if (uMsg == 26002) {
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {
			if (WSAGETSELECTEVENT(lParam) == FD_READ) {
				int data_size = 0;
				WSAAsyncSelect(gh_socket)
			}
		}
		else {
			closesocket(gh_socket);
			gh_socket = INVALID_SOCKET;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;				// ����
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);					// Ŀ��
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);						// ������
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;						// �޽��� ó����
	wc.lpszClassName = _T("SjSocketClient");					// Ŭ������ ����
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	// ������ ����
	HWND hWnd = CreateWindow(_T("SjSocketClient"), _T("Win32 Ŭ���̾�Ʈ"), 
		WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}