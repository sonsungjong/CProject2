// Win32SockServer.cpp : Defines the entry point for the application.
//

#include "pch.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "framework.h"
#include "Win32SockServer.h"

#define MAX_SEND_DATA_SIZE              1000000

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

struct ServerSocket
{
	// ����ó�� ����
	SOCKET h_listen_socket;
	// ��ſ� ����
	SOCKET h_client_socket;
};

struct ProgramData
{
	// ���� ���� ����
	ServerSocket server;
	// Ŭ���̾�Ʈ�� ������ ������
	char* p_send_data;
	// ������ �ڵ� (���������� â�� �����ͷ� ����ϰ��ϱ� ���ؼ�)
	HWND h_main_wnd;
};

// ���� ���񽺸� �����ϴ� �Լ�
void StartListenService(const TCHAR* ap_ip_address, int a_port, ProgramData* ap_app_data)
{
	// ����2.2 ���
	WSADATA temp;
	WSAStartup(0x0202, &temp);

	// AF_INET ü��� TCP���� ����
	ap_app_data->server.h_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	// IPü��, ���α׷� ��Ʈ��ȣ
	sockaddr_in addr_data = { AF_INET, htons(a_port), };
	// ��Ʈ��ũ�� ����ϵ��� ����
	InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);
	
	// ���ϰ� ��Ʈ��ũ�� ����
	bind(ap_app_data->server.h_listen_socket, (sockaddr*)&addr_data, sizeof(addr_data));
	// ������ ó���� ������ ���� (����ó��)
	listen(ap_app_data->server.h_listen_socket, 5);
	// listen���� ���Ͽ� ����ڰ� ����õ�(FD_ACCEPT)�� ���� ��
	// �񵿱�� 25001�� �޽����� �߻���Ű�� �Ѵ�
	WSAAsyncSelect(ap_app_data->server.h_listen_socket, ap_app_data->h_main_wnd, 25001, FD_ACCEPT);
}

void ProcessAccept(HWND ah_wnd)
{
	// ����� ������ ������ ����� �ּҰ��� �����´�
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);
	ServerSocket* p_server = &p_app_data->server;

	// ���� �õ��� ����(Ŭ���̾�Ʈ�� ����� ����)�� �ּҸ� ������ ����
	sockaddr_in addr_data;
	// �ڽ��� ����� �ּ� ���� ������ ũ�⸦ ���
	int addr_data_size = sizeof(addr_data);

	// Ŭ���̾�Ʈ�� ������ �㰡
	// ���� �㰡�� ���� ������ �����Ǿ� h_socket�� ����
	// addr_data �� IP�ּҰ� ���������� ����ȴ�
	SOCKET h_socket = accept(p_server->h_listen_socket, (sockaddr*)&addr_data, &addr_data_size);

	// �Ѱ��� Ŭ���̾�Ʈ�� ���
	if (p_server->h_client_socket == INVALID_SOCKET) {
		p_server->h_client_socket = h_socket;
		// TCHAR temp_ip_address[32];
		// InetNtop(AF_INET, &addr_data.sin_addr, temp_ip_address, 32);

		int temp_size = MAX_SEND_DATA_SIZE + 4, read_size = 4;
		// ���� ���ۿ� ���� ������ ũ�⸦ ����
		setsockopt(p_server->h_client_socket, SOL_SOCKET, SO_SNDBUF, (char*)&temp_size, 4);
		setsockopt(p_server->h_client_socket, SOL_SOCKET, SO_RCVBUF, (char*)&temp_size, 4);
		WSAAsyncSelect(p_server->h_client_socket, ah_wnd, 25002, FD_READ | FD_CLOSE);

		*(int*)p_app_data->p_send_data = 100000;
		send(p_server->h_client_socket, p_app_data->p_send_data, 100000 + 4, 0);
	}
	else {
		closesocket(h_socket);				// �ٸ� Ŭ���̾�Ʈ�� ������ ��� ����
	}
}

// ������ ������ �����͸� ������ �� ����ϴ� �Լ�
int ReceiveData(SOCKET ah_socket, char* ap_data, int a_size)
{
	int total_size = 0, read_size, retry_count = 0;
	// a_size��ŭ ���ŵɶ����� �ݺ��Ѵ�.
	while (total_size < a_size)
	{
		// ���� ũ�⸸ŭ �����Ѵ�
		read_size = recv(ah_socket, ap_data + total_size, a_size - total_size, 0);
		if (read_size == SOCKET_ERROR || read_size == 0) {
			Sleep(3);				// 0.003�ʸ�ŭ ���
			retry_count++;
			if (retry_count > 1000) { break; }				// 1000ȸ���� ��õ�
		}
		else {
			retry_count = 0;
			total_size += read_size;
		}
	}
	return total_size;				// ��ü ������ ����Ʈ��ŭ �����Ѵ�.
}

// ������� ���� ���ϰ� ���õ� ������ ��� �����ϴ� �Լ�
void DestroyServerSocket(HWND ah_wnd)
{
	// ���������� �����͸� �̿��ؼ� ���������� ��� GetWindowLongPtr
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);
	// ���� ����
	closesocket(p_app_data->server.h_listen_socket);
	if (p_app_data->server.h_client_socket != INVALID_SOCKET) {
		closesocket(p_app_data->server.h_client_socket);
	}
	WSACleanup();			// ���� ����
}

// ���Ͽ� �߻��ϴ� �̺�Ʈ�� ó���ϴ� �Լ�
void ProcessSocketEvent(HWND ah_wnd, SOCKET ah_socket, int a_event_type)
{
	// �ּҰ��� �����´�
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);
	if (a_event_type == FD_READ) {					// ������ ����

	}
	else {				// FD_CLOSE : Ŭ���̾�Ʈ�� ������ �����Ұ�� ������ �޽���
		closesocket(p_app_data->server.h_client_socket);
		p_app_data->server.h_client_socket = INVALID_SOCKET;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ���������� ���ֱ� ���ؼ� SetWindowLongPtr�� ����Ͽ� WM_CREATE�� ����� �� ����
	if (uMsg == WM_DESTROY) {				// WM_CLOSE �� ������ �޽���
		DestroyServerSocket(hWnd);
		PostQuitMessage(0);
	}
	else if (uMsg == 25001) {
		ProcessAccept(hWnd);				// ���ο� Ŭ���̾�Ʈ ����
	}
	else if (uMsg == 25002) {
		ProcessSocketEvent(hWnd, (SOCKET)wParam, WSAGETSELECTEVENT(lParam));
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ����ü �ʱ�ȭ
	ProgramData app_data = {
		{INVALID_SOCKET, INVALID_SOCKET},
		new char[MAX_SEND_DATA_SIZE +4], NULL
	};

	// ������ Ŭ���� ���
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;				// ����
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);					// Ŀ��
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);					// ������
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;									// �޽��� ó����
	wc.lpszClassName = _T("SjSocketServer");					// Ŭ������
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	app_data.h_main_wnd = CreateWindow(_T("SjSocketServer"), _T("��������"),
		WS_OVERLAPPEDWINDOW, 100, 90, 560, 350, NULL, NULL, hInstance, NULL);

	// ���������� ���������� (WM_CREATE ���Ұ�)
	SetWindowLongPtr(app_data.h_main_wnd, GWLP_USERDATA, (LONG_PTR)&app_data);

	// WM_CREATE�� �����ؼ� WinMain���� ������
	StartListenService(IP, PORT, &app_data);

	ShowWindow(app_data.h_main_wnd, nCmdShow);
	UpdateWindow(app_data.h_main_wnd);
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {			// WM_QUIT �޽��� �߻������� ���ѷ���
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete[] app_data.p_send_data;				// �����׽�Ʈ�� �޸� ����
	return msg.wParam;
}

