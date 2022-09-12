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
	SOCKET h_socket;				// 서버와 통신을 하기 위한 소켓
	HWND h_main_wnd;							// 메인윈도우
	HWND h_event_list;									// 상태 출력 리스트박스
	HWND h_ip_edit;										// 접속할 서버의 IP 입력
	HWND h_connect_btn, h_disconnect_btn;			// 접속과 접속해제 버튼
	HFONT h_app_font;								// 글꼴
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
			total_size += read_size;			// 수신된 크기를 합산
		}
	}
	return total_size;
}

void ConnectToServer(const TCHAR* ap_ip_address, INT a_port, HWND ah_wnd)
{
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);

	// AF_INET 주소 체계를 사용하는 TCP 방식(SOCK_STREAM)의 소켓 생성
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
		// 접속 실패 시
		closesocket(p_app_data->h_socket);
		p_app_data->h_socket = INVALID_SOCKET;
		AddEventString(p_app_data, _T("서버에 접속할 수 없습니다."));
	}
	else {
		// 접속 성공
		INT temp_size = 10000000;				// 10Mbytes로 송수신 버퍼크기 설정
		// 전송 버퍼 크기 설정
		setsockopt(p_app_data->h_socket, SOL_SOCKET, SO_SNDBUF, (CHAR*)&temp_size, sizeof(temp_size));
		// 수신 버퍼 크기 설정
		setsockopt(p_app_data->h_socket, SOL_SOCKET, SO_RCVBUF, (CHAR*)&temp_size, sizeof(temp_size));

		WSAAsyncSelect(p_app_data->h_socket, ah_wnd, 26002, FD_READ | FD_CLOSE);
		AddEventString(p_app_data, _T("서버에 접속하였습니다."));
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

	// 개발자가 처리하지 않은 메시지들을 처리
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow)
{
	// 프로그램 실행정보를 기억할 구조체
	ProgramData app_data = { INVALID_SOCKET, };

	// 글꼴 생성
	app_data.h_app_font = CreateFont(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("굴림"));

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
	DeleteObject(app_data.h_app_font);				// 글꼴 정리
	return static_cast<INT>(msg.wParam);
}

