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
	SOCKET h_listen_socket;						// 접속처리용
	SOCKET h_client_socket;						// 송수신용
};

struct ProgramData
{
	ServerSocket server;						// 서버 소켓 정보
	CHAR* p_send_data;						// 클라이언트에 데이터를 전송할 메모리
	HWND h_main_wnd;						// 메인 윈도우 핸들
	HWND h_event_list;						// 상태 출력 리스트박스
	HWND h_ip_list;								// IP리스트
	HWND h_start_btn, h_stop_btn;			// 소켓 서비스 시작 및 종료 버튼
	HFONT h_app_font;							// 글꼴
	__int64 start_tick, end_tick;				// 시간 체크
	HWND h_test_btn;
};

/* 리스트 추가 함수 */
void AddEventString(ProgramData* ap_app_data, const TCHAR* ap_str)
{
	// 리스트박스 가장 끝에 문자열 추가
	int index = ::SendMessage(ap_app_data->h_event_list, LB_INSERTSTRING, -1, (LPARAM)ap_str);
	// 리스트박스 index 위치에 있는 문자열 자동 선택
	::SendMessage(ap_app_data->h_event_list, LB_SETCURSEL, index, 0);
}

/* 소켓 리슨 서비스 시작 */
void StartListenService(const TCHAR* ap_ip_address, INT a_port, ProgramData* ap_app_data)
{
	// TCP방식의 소켓 생성
	ap_app_data->server.h_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr_data = { AF_INET, htons(a_port), };
	InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);

	bind(ap_app_data->server.h_listen_socket, (sockaddr*)&addr_data, sizeof(addr_data));
	listen(ap_app_data->server.h_listen_socket, 5);

	WSAAsyncSelect(ap_app_data->server.h_listen_socket, ap_app_data->h_main_wnd, 25001, FD_ACCEPT);

	AddEventString(ap_app_data, _T("서버 리슨 시작"));
}

void ProcessAccept(HWND ah_wnd)
{
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);
	ServerSocket* p_server = &p_app_data->server;

	// 클라이언트와 통신할 소켓의 주소를 저장하기 위한 변수
	sockaddr_in addr_data;
	// 사용할 주소 저장 변수의 크기를 계산
	INT addr_data_size = sizeof(addr_data);

	// 접속 허용
	SOCKET h_socket = accept(p_server->h_listen_socket, (sockaddr*)&addr_data, &addr_data_size);

	// 클라이언트 정보 저장
	if (p_server->h_client_socket == INVALID_SOCKET)
	{
		p_server->h_client_socket = h_socket;
		TCHAR temp_ip_address[32], msg_str[128];
		InetNtop(AF_INET, &addr_data.sin_addr, temp_ip_address, 32);
		_stprintf_s(msg_str, 128, _T("[ %s ]에서 사용자가 접속했습니다."), temp_ip_address);
		AddEventString(p_app_data, msg_str);

		INT temp_size = MAX_SEND_DATA_SIZE +4;
		INT read_size = 4;
		setsockopt(p_server->h_client_socket, SOL_SOCKET, SO_SNDBUF, (char*)&temp_size, 4);
		setsockopt(p_server->h_client_socket, SOL_SOCKET, SO_RCVBUF, (char*)&temp_size, 4);

		// 접속하면 25002를 발생시켜라
		WSAAsyncSelect(p_server->h_client_socket, ah_wnd, 25002, FD_READ | FD_CLOSE);

		AddEventString(p_app_data, _T("클라이언트에 100000바이트 데이터를 전송합니다."));

		*(INT*)p_app_data->p_send_data = 100000;
		send(p_server->h_client_socket, p_app_data->p_send_data, 100000 + 4, 0);
	}
	else {
		closesocket(h_socket);
	}
}

// recv함수 개조 -> 수신 실패했을 경우 재시도하게끔 변경
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
			total_size += read_size;			// 수신된 크기를 합산
		}
	}
	return total_size;
}

/* 서버 소켓 관련 정보를 모두 제거 */
void DestroyServerSocket(HWND ah_wnd)
{
	// 윈도우의 사용자 데이터 영역에 저장된 주소값을 가져옴 (전역변수 대체)
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
		AddEventString(p_app_data, _T("서버를 중지합니다"));
	}
}

/* 소켓에 발생하는 이벤트를 처리 */
void ProcessSocketEvent(HWND ah_wnd, SOCKET ah_socket, INT a_event_type)
{
	// 윈도우의 사용자 데이터 영역에 저장된 주소값을 가져옴 (전역변수 대체)
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);

	if (a_event_type == FD_READ) {
		// 데이터 수신

	}
	else {
		// 클라이언트 접속 해제
		closesocket(p_app_data->server.h_client_socket);
		p_app_data->server.h_client_socket = INVALID_SOCKET;
		AddEventString(p_app_data, _T("클라이언트가 접속을 해제했습니다."));
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

	// 대상 컨트롤의 아이디 얻기
	int ctrl_id = LOWORD(wParam);
	if (ctrl_id == 2100) {
		// 리슨 서비스 중단이면 실행
		if (p_app_data->server.h_listen_socket == INVALID_SOCKET) {
			TCHAR ip_address[32];
			INT index = ::SendMessage(p_app_data->h_ip_list, CB_GETCURSEL, 0, 0);
			if (index != CB_ERR) {
				::SendMessage(p_app_data->h_ip_list, CB_GETLBTEXT, index, (LPARAM)ip_address);
			}
			//리슨 서비스 시작
			StartListenService(IP, PORT, p_app_data);
		}
	}
	else if (ctrl_id == 2101) {
		// 리슨 서비스 실행을 중단
		if (p_app_data->server.h_listen_socket != INVALID_SOCKET) {
			DestroyServerSocket(hWnd);
		}
	}
	else if (ctrl_id == 2102) {
		if (p_app_data->server.h_client_socket != INVALID_SOCKET) {
			AddEventString(p_app_data, _T("클라이언트에 4400000바이트 데이터를 전송합니다"));
			*(int*)p_app_data->p_send_data = 4400000;
			p_app_data->start_tick = GetTickCount64();
			send(p_app_data->server.h_client_socket, p_app_data->p_send_data, 4400000 + 4, 0);
		}
	}
}

// 현재 서버가 실행된 컴퓨터에서 사용 가능한 IP목록을 얻는 함수
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

	gethostname(host_name, 128);							// 현재 컴퓨터의 호스트 명칭을 얻음

	if (!getaddrinfo(host_name, NULL, &hints, &result)) {
		// 호스트의 IP정보를 얻음
		ptr = result;
		UCHAR *p_pos;
		while (ptr != NULL) {
			p_pos = &((sockaddr_in*)ptr->ai_addr)->sin_addr.S_un.S_un_b.s_b1;
			_stprintf_s(ip_str, 24, _T("%hhu.%hhu.%hhu.%hhu"), *p_pos, *(p_pos + 1), *(p_pos + 2), *(p_pos + 3));
			::SendMessage(ah_ip_list, CB_INSERTSTRING, -1, (LPARAM)ip_str);
			// 다음 IP 정보로 이동
			ptr = ptr->ai_next;
		}
		freeaddrinfo(result);
	}
	::SendMessage(ah_ip_list, CB_SETCURSEL, 0, 0);
}

void CreateControl(ProgramData* ap_app_data) {
	// 리스트박스 생성
	ap_app_data->h_event_list = CreateWindow(_T("Listbox"), NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_NOINTEGRALHEIGHT,
		4, 4, 536, 263, ap_app_data->h_main_wnd, (HMENU)2000, 0, 0);
	// 리스트박스에 글꼴 적용
	SendMessage(ap_app_data->h_ip_list, WM_SETFONT, (WPARAM)ap_app_data->h_app_font, 0);

	// 서비스 가능한 주소 목록 출력을 위해 콤보 박스 생성
	ap_app_data->h_ip_list = CreateWindow(_T("Conbobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT,
		4, 274, 150, 300, ap_app_data->h_main_wnd, (HMENU)2001, 0, 0);
	// 생성된 콤보 박스에 글꼴 적용
	SendMessage(ap_app_data->h_ip_list, WM_SETFONT, (WPARAM)ap_app_data->h_app_font, 0);
	// 서비스 가능한 주소 목록 구성
	GetLocalNetworkAddress(ap_app_data->h_ip_list);

	// 버튼 생성
	ap_app_data->h_start_btn = CreateWindow(_T("Button"), _T("서비스 시작"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		160, 271, 90, 28, ap_app_data->h_main_wnd, (HMENU)2100, 0, 0);
	// 생성된 버튼에 글꼴 적용
	SendMessage(ap_app_data->h_start_btn, WM_SETFONT, (WPARAM)ap_app_data->h_app_font, 0);

	// 버튼 생성
	ap_app_data->h_stop_btn = CreateWindow(_T("Button"), _T("서비스 멈춤"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		253, 271, 90, 28, ap_app_data->h_main_wnd, (HMENU)2101, 0,0);
	// 글꼴 적용
	SendMessage(ap_app_data->h_stop_btn, WM_SETFONT, (WPARAM)ap_app_data->h_app_font, 0);

	// 테스트 버튼 생성
	ap_app_data->h_test_btn = CreateWindow(_T("Button"), _T("테스트 전송"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		346, 271, 90, 28, ap_app_data->h_main_wnd, (HMENU)2102, 0, 0);
	// 글꼴 적용
	SendMessage(ap_app_data->h_test_btn, WM_SETFONT, (WPARAM)ap_app_data->h_app_font, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY)
	{
		// 서버 소켓 관련 정보를 모두 제거
		DestroyServerSocket(hWnd);
		// 컨트롤 제거
		DestroyControl(hWnd);
		// 메시지 큐에 WM_QUIT
		PostQuitMessage(0);
	}
	else if (msg == 25001) {
		// 클라이언트 접속
		ProcessAccept(hWnd);
	}
	else if (msg == 25002) {
		// 데이터 수신 혹은 클라이언트 접속 해제
		ProcessSocketEvent(hWnd, (SOCKET)wParam, WSAGETSELECTEVENT(lParam));
	}
	else if (msg == WM_COMMAND) {
		OnCommand(hWnd, wParam, lParam);
	}

	// 개발자가 처리하지 않은 메시지들을 처리
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow)
{
	// 프로그램 실행 정보를 기억할 구조체
	ProgramData app_data = {
		{INVALID_SOCKET, INVALID_SOCKET}, new char[MAX_SEND_DATA_SIZE + 4], NULL
	};

	// 글꼴 지정
	app_data.h_app_font = CreateFont(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("굴림"));

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

	// 전역변수 제거를 위해 포인터 활용 (GetWindowLongPtr 로 사용 가능)
	SetWindowLongPtr(app_data.h_main_wnd, GWLP_USERDATA, (LONG_PTR)&app_data);

	// 컨트롤 생성
	CreateControl(&app_data);

	ShowWindow(app_data.h_main_wnd, nCmdShow);
	UpdateWindow(app_data.h_main_wnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete[] app_data.p_send_data;					// 전송테스트용 메모리 제거
	DeleteObject(app_data.h_app_font);
	WSACleanup();

	return static_cast<INT>(msg.wParam);
}

