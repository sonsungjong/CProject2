#include "pch.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "framework.h"
#include "SocKetTCPServer.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

#define IP						_T("222.100.255.148")
#define PORT					19000
#define MAX_SEND_DATA_SIZE			100000

struct ServerSocket 
{
	SOCKET h_listen_socket;						// ����ó����
	SOCKET h_client_socket;						// �ۼ��ſ�
};

struct ProgramData
{
	ServerSocket server;						// ���� ���� ����
	CHAR* p_send_data;						// Ŭ���̾�Ʈ�� �����͸� ������ �޸�
	HWND h_main_wnd;						// ���� ������ �ڵ�
	HWND h_event_list;						// ���� ��� ����Ʈ�ڽ�
	HWND h_ip_list;								// IP����Ʈ
	HWND h_start_btn, h_stop_btn;			// ���� ���� ���� �� ���� ��ư
	HFONT h_app_font;							// �۲�
	__int64 start_tick, end_tick;				// �ð� üũ
	HWND h_test_btn;
};

/* ����Ʈ �߰� �Լ� */
void AddEventString(ProgramData* ap_app_data, const TCHAR* ap_str)
{
	// ����Ʈ�ڽ� ���� ���� ���ڿ� �߰�
	int index = ::SendMessage(ap_app_data->h_event_list, LB_INSERTSTRING, -1, (LPARAM)ap_str);
	// ����Ʈ�ڽ� index ��ġ�� �ִ� ���ڿ� �ڵ� ����
	::SendMessage(ap_app_data->h_event_list, LB_SETCURSEL, index, 0);
}

/* ���� ���� ���� ���� */
void StartListenService(const TCHAR* ap_ip_address, INT a_port, ProgramData* ap_app_data)
{
	// TCP����� ���� ����
	ap_app_data->server.h_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr_data = { AF_INET, htons(a_port), };
	InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);

	bind(ap_app_data->server.h_listen_socket, (sockaddr*)&addr_data, sizeof(addr_data));
	listen(ap_app_data->server.h_listen_socket, 5);

	WSAAsyncSelect(ap_app_data->server.h_listen_socket, ap_app_data->h_main_wnd, 25001, FD_ACCEPT);

	AddEventString(ap_app_data, _T("���� ���� ����"));
}

void ProcessAccept(HWND ah_wnd)
{
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);
	ServerSocket* p_server = &p_app_data->server;

	// Ŭ���̾�Ʈ�� ����� ������ �ּҸ� �����ϱ� ���� ����
	sockaddr_in addr_data;
	// ����� �ּ� ���� ������ ũ�⸦ ���
	INT addr_data_size = sizeof(addr_data);

	// ���� ���
	SOCKET h_socket = accept(p_server->h_listen_socket, (sockaddr*)&addr_data, &addr_data_size);

	// Ŭ���̾�Ʈ ���� ����
	if (p_server->h_client_socket == INVALID_SOCKET)
	{
		p_server->h_client_socket = h_socket;
		TCHAR temp_ip_address[32], msg_str[128];
		InetNtop(AF_INET, &addr_data.sin_addr, temp_ip_address, 32);
		_stprintf_s(msg_str, 128, _T("[ %s ]���� ����ڰ� �����߽��ϴ�."), temp_ip_address);
		AddEventString(p_app_data, msg_str);

		INT temp_size = MAX_SEND_DATA_SIZE +4;
		INT read_size = 4;
		setsockopt(p_server->h_client_socket, SOL_SOCKET, SO_SNDBUF, (char*)&temp_size, 4);
		setsockopt(p_server->h_client_socket, SOL_SOCKET, SO_RCVBUF, (char*)&temp_size, 4);

		// �����ϸ� 25002�� �߻����Ѷ�
		WSAAsyncSelect(p_server->h_client_socket, ah_wnd, 25002, FD_READ | FD_CLOSE);

		AddEventString(p_app_data, _T("Ŭ���̾�Ʈ�� 100000����Ʈ �����͸� �����մϴ�."));

		*(INT*)p_app_data->p_send_data = 100000;
		send(p_server->h_client_socket, p_app_data->p_send_data, 100000 + 4, 0);
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
			Sleep(3);
			retry_count++;
			if (retry_count > 1000) {
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

/* ���� ���� ���� ������ ��� ���� */
void DestroyServerSocket(HWND ah_wnd)
{
	// �������� ����� ������ ������ ����� �ּҰ��� ������ (�������� ��ü)
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);
	
	if (p_app_data->server.h_listen_socket != INVALID_SOCKET)
	{
		closesocket(p_app_data->server.h_listen_socket);
		p_app_data->server.h_listen_socket = INVALID_SOCKET;
		if (p_app_data->server.h_client_socket != INVALID_SOCKET)
		{
			closesocket(p_app_data->server.h_client_socket);
			p_app_data->server.h_client_socket = INVALID_SOCKET;
		}
		AddEventString(p_app_data, _T("������ �����մϴ�"));
	}
}

/* ���Ͽ� �߻��ϴ� �̺�Ʈ�� ó�� */
void ProcessSocketEvent(HWND ah_wnd, SOCKET ah_socket, INT a_event_type)
{
	// �������� ����� ������ ������ ����� �ּҰ��� ������ (�������� ��ü)
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);

	if (a_event_type == FD_READ) {
		// ������ ����

	}
	else {
		// Ŭ���̾�Ʈ ���� ����
		closesocket(p_app_data->server.h_client_socket);
		p_app_data->server.h_client_socket = INVALID_SOCKET;
		AddEventString(p_app_data, _T("Ŭ���̾�Ʈ�� ������ �����߽��ϴ�."));
	}
}

void DestroyControl(HWND ah_wnd)
{
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);
	DestroyWindow(p_app_data->h_event_list);
	DestroyWindow(p_app_data->h_ip_list);
	DestroyWindow(p_app_data->h_start_btn);
	DestroyWindow(p_app_data->h_stop_btn);
	DestroyWindow(p_app_data->h_test_btn);
}

void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	// ��� ��Ʈ���� ���̵� ���
	int ctrl_id = LOWORD(wParam);
	if (ctrl_id == 2100) {
		// ���� ���� �ߴ��̸� ����
		if (p_app_data->server.h_listen_socket == INVALID_SOCKET) {
			TCHAR ip_address[32];
			INT index = ::SendMessage(p_app_data->h_ip_list, CB_GETCURSEL, 0, 0);
			if (index != CB_ERR) {
				::SendMessage(p_app_data->h_ip_list, CB_GETLBTEXT, index, (LPARAM)ip_address);
			}
			//���� ���� ����
			StartListenService(IP, PORT, p_app_data);
		}
	}
	else if (ctrl_id == 2101) {
		// ���� ���� ������ �ߴ�
		if (p_app_data->server.h_listen_socket != INVALID_SOCKET) {
			DestroyServerSocket(hWnd);
		}
	}
	else if (ctrl_id == 2102) {
		if (p_app_data->server.h_client_socket != INVALID_SOCKET) {
			AddEventString(p_app_data, _T("Ŭ���̾�Ʈ�� 4400000����Ʈ �����͸� �����մϴ�"));
			*(int*)p_app_data->p_send_data = 4400000;
			p_app_data->start_tick = GetTickCount64();
			send(p_app_data->server.h_client_socket, p_app_data->p_send_data, 4400000 + 4, 0);
		}
	}
}

// ���� ������ ����� ��ǻ�Ϳ��� ��� ������ IP����� ��� �Լ�
void GetLocalNetworkAddress(HWND ah_ip_list)
{
	INT count = 0;
	TCHAR ip_str[24];
	CHAR host_name[128] = { 0, };
	addrinfo *result = NULL, *ptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	gethostname(host_name, 128);							// ���� ��ǻ���� ȣ��Ʈ ��Ī�� ����

	if (!getaddrinfo(host_name, NULL, &hints, &result)) {
		// ȣ��Ʈ�� IP������ ����
		ptr = result;
		UCHAR *p_pos;
		while (ptr != NULL) {
			p_pos = &((sockaddr_in*)ptr->ai_addr)->sin_addr.S_un.S_un_b.s_b1;
			_stprintf_s(ip_str, 24, _T("%hhu.%hhu.%hhu.%hhu"), *p_pos, *(p_pos + 1), *(p_pos + 2), *(p_pos + 3));
			::SendMessage(ah_ip_list, CB_INSERTSTRING, -1, (LPARAM)ip_str);
			// ���� IP ������ �̵�
			ptr = ptr->ai_next;
		}
		freeaddrinfo(result);
	}
	::SendMessage(ah_ip_list, CB_SETCURSEL, 0, 0);
}

void CreateControl(ProgramData* ap_app_data) {
	// ����Ʈ�ڽ� ����
	ap_app_data->h_event_list = CreateWindow(_T("Listbox"), NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_NOINTEGRALHEIGHT,
		4, 4, 536, 263, ap_app_data->h_main_wnd, (HMENU)2000, 0, 0);
	// ����Ʈ�ڽ��� �۲� ����
	SendMessage(ap_app_data->h_ip_list, WM_SETFONT, (WPARAM)ap_app_data->h_app_font, 0);

	// ���� ������ �ּ� ��� ����� ���� �޺� �ڽ� ����
	ap_app_data->h_ip_list = CreateWindow(_T("Conbobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT,
		4, 274, 150, 300, ap_app_data->h_main_wnd, (HMENU)2001, 0, 0);
	// ������ �޺� �ڽ��� �۲� ����
	SendMessage(ap_app_data->h_ip_list, WM_SETFONT, (WPARAM)ap_app_data->h_app_font, 0);
	// ���� ������ �ּ� ��� ����
	GetLocalNetworkAddress(ap_app_data->h_ip_list);

	// ��ư ����
	ap_app_data->h_start_btn = CreateWindow(_T("Button"), _T("���� ����"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		160, 271, 90, 28, ap_app_data->h_main_wnd, (HMENU)2100, 0, 0);
	// ������ ��ư�� �۲� ����
	SendMessage(ap_app_data->h_start_btn, WM_SETFONT, (WPARAM)ap_app_data->h_app_font, 0);

	// ��ư ����
	ap_app_data->h_stop_btn = CreateWindow(_T("Button"), _T("���� ����"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		253, 271, 90, 28, ap_app_data->h_main_wnd, (HMENU)2101, 0,0);
	// �۲� ����
	SendMessage(ap_app_data->h_stop_btn, WM_SETFONT, (WPARAM)ap_app_data->h_app_font, 0);

	// �׽�Ʈ ��ư ����
	ap_app_data->h_test_btn = CreateWindow(_T("Button"), _T("�׽�Ʈ ����"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		346, 271, 90, 28, ap_app_data->h_main_wnd, (HMENU)2102, 0, 0);
	// �۲� ����
	SendMessage(ap_app_data->h_test_btn, WM_SETFONT, (WPARAM)ap_app_data->h_app_font, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY)
	{
		// ���� ���� ���� ������ ��� ����
		DestroyServerSocket(hWnd);
		// ��Ʈ�� ����
		DestroyControl(hWnd);
		// �޽��� ť�� WM_QUIT
		PostQuitMessage(0);
	}
	else if (msg == 25001) {
		// Ŭ���̾�Ʈ ����
		ProcessAccept(hWnd);
	}
	else if (msg == 25002) {
		// ������ ���� Ȥ�� Ŭ���̾�Ʈ ���� ����
		ProcessSocketEvent(hWnd, (SOCKET)wParam, WSAGETSELECTEVENT(lParam));
	}
	else if (msg == WM_COMMAND) {
		OnCommand(hWnd, wParam, lParam);
	}

	// �����ڰ� ó������ ���� �޽������� ó��
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow)
{
	// ���α׷� ���� ������ ����� ����ü
	ProgramData app_data = {
		{INVALID_SOCKET, INVALID_SOCKET}, new char[MAX_SEND_DATA_SIZE + 4], NULL
	};

	// �۲� ����
	app_data.h_app_font = CreateFont(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("����"));

	WSADATA temp;
	(void)WSAStartup(0x0202, &temp);

	WNDCLASS wc;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = _T("SockServer123");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	app_data.h_main_wnd = CreateWindow(_T("SockServer123"), _T("Win32 Server Socket"), WS_OVERLAPPEDWINDOW, 100, 90, 560, 350, NULL, NULL, hInstance, NULL);

	// �������� ���Ÿ� ���� ������ Ȱ�� (GetWindowLongPtr �� ��� ����)
	SetWindowLongPtr(app_data.h_main_wnd, GWLP_USERDATA, (LONG_PTR)&app_data);

	// ��Ʈ�� ����
	CreateControl(&app_data);

	ShowWindow(app_data.h_main_wnd, nCmdShow);
	UpdateWindow(app_data.h_main_wnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete[] app_data.p_send_data;					// �����׽�Ʈ�� �޸� ����
	DeleteObject(app_data.h_app_font);
	WSACleanup();

	return static_cast<INT>(msg.wParam);
}

