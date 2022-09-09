#include "pch.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "framework.h"
#include "SocketTCPClient.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

#define IP					_T("222.100.255.148")
#define PORT				19000

SOCKET gh_socket = INVALID_SOCKET;

INT ReceiveData(CHAR* ap_data, INT a_size)
{
	INT total_size = 0;
	INT read_size = 0;
	INT retry_count = 0;
	while (total_size < a_size)
	{
		read_size = recv(gh_socket, ap_data + total_size, a_size - total_size, 0);
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
			total_size += read_size;			// 수신된 크기를 합산
		}
	}
	return total_size;
}

void ConnectToServer(const TCHAR* ap_ip_address, INT a_port, HWND ah_wnd)
{
	WSADATA temp;
	WSAStartup(0x0202, &temp);

	gh_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr_data = { AF_INET, htons(a_port), };
	InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);
	WSAAsyncSelect(gh_socket, ah_wnd, 26001, FD_CONNECT);
	connect(gh_socket, (sockaddr*)&addr_data, sizeof(addr_data));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY)
	{
		closesocket(gh_socket);

		PostQuitMessage(0);
	}
	else if (msg == WM_CREATE) {
		ConnectToServer(IP, PORT, hWnd);								// 리슨서비스 시작
	}
	else if (msg == 26001) {				// 클라이언트 접속
		if (WSAGETSELECTERROR(lParam)) {
			closesocket(gh_socket);
			gh_socket = INVALID_SOCKET;
		}
		else {			// 접속 성공
			INT temp_size = 200000;
			INT read_size = 4;
			setsockopt(gh_socket, SOL_SOCKET, SO_SNDBUF, (CHAR*)&temp_size, 4);
			setsockopt(gh_socket, SOL_SOCKET, SO_RCVBUF, (CHAR*)&temp_size, 4);
			WSAAsyncSelect(gh_socket, hWnd, 26002, FD_READ | FD_CLOSE);
		}
	}
	else if (msg == 26002) {
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {
			// 데이터 수신
			INT data_size = 0;
			WSAAsyncSelect(gh_socket, hWnd, 26002, FD_CLOSE);
			ReceiveData((CHAR*)&data_size, 4);
			CHAR* p_data = new CHAR[data_size];
			INT read_size = ReceiveData(p_data, data_size);

			HDC h_dc = GetDC(hWnd);
			TCHAR msg_str[128];
			INT len = _stprintf_s(msg_str, 128, _T("데이터 수신 : %d / %d"), read_size, data_size);
			SetBkMode(h_dc, TRANSPARENT);
			TextOut(h_dc, 10, 10, msg_str, len);
			ReleaseDC(hWnd, h_dc);
			delete[] p_data;

			WSAAsyncSelect(gh_socket, hWnd, 26002, FD_READ | FD_CLOSE);
		}
		else {
			// 서버가 접속을 끊음
			closesocket(gh_socket);
			gh_socket = INVALID_SOCKET;
		}
	}
	// 개발자가 처리하지 않은 메시지들을 처리
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
	wc.lpszClassName = _T("SockClient123");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	HWND hWnd = CreateWindow(_T("SockClient123"), _T("Win32 Client Socket"), WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<INT>(msg.wParam);
}

