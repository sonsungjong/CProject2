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
	// 접속처리 소켓
	SOCKET h_listen_socket;
	// 통신용 소켓
	SOCKET h_client_socket;
};

struct ProgramData
{
	// 서버 소켓 정보
	ServerSocket server;
	// 클라이언트로 전송할 데이터
	char* p_send_data;
	// 윈도우 핸들 (지역변수를 창에 포인터로 사용하게하기 위해서)
	HWND h_main_wnd;
};

// 리슨 서비스를 시작하는 함수
void StartListenService(const TCHAR* ap_ip_address, int a_port, ProgramData* ap_app_data)
{
	// 소켓2.2 사용
	WSADATA temp;
	WSAStartup(0x0202, &temp);

	// AF_INET 체계로 TCP소켓 생성
	ap_app_data->server.h_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	// IP체계, 프로그램 포트번호
	sockaddr_in addr_data = { AF_INET, htons(a_port), };
	// 네트워크를 사용하도록 설정
	InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);
	
	// 소켓과 네트워크를 연결
	bind(ap_app_data->server.h_listen_socket, (sockaddr*)&addr_data, sizeof(addr_data));
	// 접속을 처리할 단위를 설정 (순차처리)
	listen(ap_app_data->server.h_listen_socket, 5);
	// listen중인 소켓에 사용자가 연결시도(FD_ACCEPT)를 했을 때
	// 비동기로 25001번 메시지를 발생시키게 한다
	WSAAsyncSelect(ap_app_data->server.h_listen_socket, ap_app_data->h_main_wnd, 25001, FD_ACCEPT);
}

void ProcessAccept(HWND ah_wnd)
{
	// 사용자 데이터 영역에 저장된 주소값을 가져온다
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);
	ServerSocket* p_server = &p_app_data->server;

	// 접속 시도한 소켓(클라이언트와 통신할 소켓)의 주소를 저장할 변수
	sockaddr_in addr_data;
	// 자신의 사용할 주소 저장 변수의 크기를 계산
	int addr_data_size = sizeof(addr_data);

	// 클라이언트의 접속을 허가
	// 접속 허가시 복제 소켓이 생성되어 h_socket에 저장
	// addr_data 에 IP주소가 정수값으로 저장된다
	SOCKET h_socket = accept(p_server->h_listen_socket, (sockaddr*)&addr_data, &addr_data_size);

	// 한개의 클라이언트만 사용
	if (p_server->h_client_socket == INVALID_SOCKET) {
		p_server->h_client_socket = h_socket;
		// TCHAR temp_ip_address[32];
		// InetNtop(AF_INET, &addr_data.sin_addr, temp_ip_address, 32);

		int temp_size = MAX_SEND_DATA_SIZE + 4, read_size = 4;
		// 전송 버퍼와 수신 버퍼의 크기를 설정
		setsockopt(p_server->h_client_socket, SOL_SOCKET, SO_SNDBUF, (char*)&temp_size, 4);
		setsockopt(p_server->h_client_socket, SOL_SOCKET, SO_RCVBUF, (char*)&temp_size, 4);
		WSAAsyncSelect(p_server->h_client_socket, ah_wnd, 25002, FD_READ | FD_CLOSE);

		*(int*)p_app_data->p_send_data = 100000;
		send(p_server->h_client_socket, p_app_data->p_send_data, 100000 + 4, 0);
	}
	else {
		closesocket(h_socket);				// 다른 클라이언트와 서버가 통신 중임
	}
}

// 서버가 전송한 데이터를 수신할 때 사용하는 함수
int ReceiveData(SOCKET ah_socket, char* ap_data, int a_size)
{
	int total_size = 0, read_size, retry_count = 0;
	// a_size만큼 수신될때까지 반복한다.
	while (total_size < a_size)
	{
		// 남은 크기만큼 수신한다
		read_size = recv(ah_socket, ap_data + total_size, a_size - total_size, 0);
		if (read_size == SOCKET_ERROR || read_size == 0) {
			Sleep(3);				// 0.003초만큼 대기
			retry_count++;
			if (retry_count > 1000) { break; }				// 1000회까지 재시도
		}
		else {
			retry_count = 0;
			total_size += read_size;
		}
	}
	return total_size;				// 전체 수신한 바이트만큼 리턴한다.
}

// 사용중인 서버 소켓과 관련된 정보를 모두 제거하는 함수
void DestroyServerSocket(HWND ah_wnd)
{
	// 전역변수를 포인터를 이용해서 지역변수로 사용 GetWindowLongPtr
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);
	// 소켓 종료
	closesocket(p_app_data->server.h_listen_socket);
	if (p_app_data->server.h_client_socket != INVALID_SOCKET) {
		closesocket(p_app_data->server.h_client_socket);
	}
	WSACleanup();			// 소켓 제거
}

// 소켓에 발생하는 이벤트를 처리하는 함수
void ProcessSocketEvent(HWND ah_wnd, SOCKET ah_socket, int a_event_type)
{
	// 주소값을 가져온다
	ProgramData* p_app_data = (ProgramData*)GetWindowLongPtr(ah_wnd, GWLP_USERDATA);
	if (a_event_type == FD_READ) {					// 데이터 수신

	}
	else {				// FD_CLOSE : 클라이언트가 연결을 해제할경우 들어오는 메시지
		closesocket(p_app_data->server.h_client_socket);
		p_app_data->server.h_client_socket = INVALID_SOCKET;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 전역변수를 없애기 위해서 SetWindowLongPtr을 사용하여 WM_CREATE를 사용할 수 없음
	if (uMsg == WM_DESTROY) {				// WM_CLOSE 후 들어오는 메시지
		DestroyServerSocket(hWnd);
		PostQuitMessage(0);
	}
	else if (uMsg == 25001) {
		ProcessAccept(hWnd);				// 새로운 클라이언트 접속
	}
	else if (uMsg == 25002) {
		ProcessSocketEvent(hWnd, (SOCKET)wParam, WSAGETSELECTEVENT(lParam));
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 구조체 초기화
	ProgramData app_data = {
		{INVALID_SOCKET, INVALID_SOCKET},
		new char[MAX_SEND_DATA_SIZE +4], NULL
	};

	// 윈도우 클래스 등록
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;				// 배경색
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);					// 커서
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);					// 아이콘
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;									// 메시지 처리기
	wc.lpszClassName = _T("SjSocketServer");					// 클래스명
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	app_data.h_main_wnd = CreateWindow(_T("SjSocketServer"), _T("서버소켓"),
		WS_OVERLAPPEDWINDOW, 100, 90, 560, 350, NULL, NULL, hInstance, NULL);

	// 전역변수를 지역변수로 (WM_CREATE 사용불가)
	SetWindowLongPtr(app_data.h_main_wnd, GWLP_USERDATA, (LONG_PTR)&app_data);

	// WM_CREATE를 사용못해서 WinMain으로 가져옴
	StartListenService(IP, PORT, &app_data);

	ShowWindow(app_data.h_main_wnd, nCmdShow);
	UpdateWindow(app_data.h_main_wnd);
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {			// WM_QUIT 메시지 발생전까지 무한루프
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete[] app_data.p_send_data;				// 전송테스트용 메모리 제거
	return msg.wParam;
}

