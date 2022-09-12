#include "pch.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "framework.h"
#include "SocketTCPClient.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

#define IP					_T("222.100.255.148")
#define PORT				19000

struct ProgramData
{
	SOCKET h_socket;				// ������ ����� �ϱ� ���� ����
	HWND h_main_wnd;							// ����������
	HWND h_event_list;									// ���� ��� ����Ʈ�ڽ�
	HWND h_ip_edit;										// ������ ������ IP �Է�
	HWND h_connect_btn, h_disconnect_btn;			// ���Ӱ� �������� ��ư
	HFONT h_app_font;								// �۲�
};

void AddEventString(ProgramData* ap_app_data, const TCHAR* ap_str)
{
	INT index = ::SendMessage(ap_app_data->h_event_list, LB_INSERTSTRING, -1, (LPARAM)ap_str);
	::SendMessage(ap_app_data->h_event_list, LB_SETCURSEL, index, 0);
}

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
			Sleep(1);
			retry_count++;
			if (retry_count > 2000) {
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

void ConnectToServer(const TCHAR* ap_ip_address, INT a_port, HWND ah_wnd)
{
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);

	// AF_INET �ּ� ü�踦 ����ϴ� TCP ���(SOCK_STREAM)�� ���� ����
	p_app_data->h_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr_data = { AF_INET, htons(a_port), };
	InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);
	WSAAsyncSelect(p_app_data->h_socket, p_app_data->h_main_wnd, 26001, FD_CONNECT);
	connect(p_app_data->h_socket, (sockaddr*)&addr_data, sizeof(addr_data));
}

void OnDestroy(HWND ah_wnd)
{
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);
	if (p_app_data->h_socket != INVALID_SOCKET) {
		closesocket(p_app_data->h_socket);
	}

	DestroyWindow(p_app_data->h_event_list);
	DestroyWindow(p_app_data->h_ip_edit);
	DestroyWindow(p_app_data->h_connect_btn);
	DestroyWindow(p_app_data->h_disconnect_btn);

	PostQuitMessage(0);
}

void OnProcessConnectionResult(HWND ah_wnd, INT a_error_code)
{
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);

	if (a_error_code) {
		// ���� ���� ��
		closesocket(p_app_data->h_socket);
		p_app_data->h_socket = INVALID_SOCKET;
		AddEventString(p_app_data, _T("������ ������ �� �����ϴ�."));
	}
	else {
		// ���� ����
		INT temp_size = 10000000;				// 10Mbytes�� �ۼ��� ����ũ�� ����
		// ���� ���� ũ�� ����
		setsockopt(p_app_data->h_socket, SOL_SOCKET, SO_SNDBUF, (CHAR*)&temp_size, sizeof(temp_size));
		// ���� ���� ũ�� ����
		setsockopt(p_app_data->h_socket, SOL_SOCKET, SO_RCVBUF, (CHAR*)&temp_size, sizeof(temp_size));

		WSAAsyncSelect(p_app_data->h_socket, ah_wnd, 26002, FD_READ | FD_CLOSE);
		AddEventString(p_app_data, _T("������ �����Ͽ����ϴ�."));
	}
}

void OnSocketMessage(HWND ah_wnd, INT a_notify_type)
{

}

void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}

void CreateControl(ProgramData *ap_app_data)
{

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY) {
		OnDestroy(hWnd);
	}
	else if (msg == WM_COMMAND) {
		OnCommand(hWnd, wParam, lParam);
	}
	else if (msg == 26001) {
		OnProcessConnectionResult(hWnd, WSAGETSELECTERROR(lParam));
	}
	else if (msg == 26002) {
		OnSocketMessage(hWnd, WSAGETSELECTEVENT(lParam));
	}

	// �����ڰ� ó������ ���� �޽������� ó��
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow)
{
	// ���α׷� ���������� ����� ����ü
	ProgramData app_data = { INVALID_SOCKET, };

	// �۲� ����
	app_data.h_app_font = CreateFont(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("����"));

	WSADATA temp;
	(void)WSAStartup(0x02020, &temp);

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

	app_data.h_main_wnd = CreateWindow(_T("SockClient123"), _T("Win32 Client Socket"), WS_OVERLAPPEDWINDOW, 100, 90, 560, 350, NULL, NULL, hInstance, NULL);

	SetWindowLongPtr(app_data.h_main_wnd, GWLP_USERDATA, (LONG_PTR)&app_data);
	CreateControl(&app_data);

	ShowWindow(app_data.h_main_wnd, nCmdShow);
	UpdateWindow(app_data.h_main_wnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	WSACleanup();
	DeleteObject(app_data.h_app_font);				// �۲� ����
	return static_cast<INT>(msg.wParam);
}

