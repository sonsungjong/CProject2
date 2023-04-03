#include "MyServer.h"

MyServer::MyServer()
{
	InitWinSock();
}

MyServer::~MyServer()
{

}

void MyServer::InitWinSock()
{
	WSADATA wsa_data;
	int result = WSAStartup(0x0202, &wsa_data);

	if (result != 0) {
		// 소켓 라이브러리 초기화 실패 로그

	}

	// 클라이언트 리스트 초기화
	for (int i = 0; i < MAX_USER; ++i)
	{
		m_user[i].h_socket = INVALID_SOCKET;			// -1
	}

	mh_listen_socket = INVALID_SOCKET;				// 리슨용 소켓 초기화
	memset(m_msg, 0, MSG_SIZE);
	mp_net_user = NULL;

	InitOverride();				// 프레임워크에 맞게 초기화작업
}

void MyServer::CreateSocket()
{
	mh_listen_socket = socket(AF_INET, SOCK_STREAM, 0);				// 소켓 생성
	if (mh_listen_socket == INVALID_SOCKET)
	{
		// 소켓 생성 실패 로그
		WSAGetLastError();

		WSACleanup();
	}
	CreateOverride();				// 프레임워크에 맞게 소켓을 사용할 화면과 메시지번호를 지정
}

void MyServer::BindSocket(const TCHAR* a_ipv4, int a_port)
{
	sockaddr_in server_addr = { AF_INET, htons(a_port), };
	InetPton(AF_INET, a_ipv4, &server_addr.sin_addr.s_addr);

	int result = bind(mh_listen_socket, (sockaddr*)&server_addr, sizeof(server_addr));
	if (result == SOCKET_ERROR)
	{
		// 바인드 실패 로그
		WSAGetLastError();

		closesocket(mh_listen_socket);
		WSACleanup();
	}
}

void MyServer::StartListen()
{
	int result = listen(mh_listen_socket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		// 리슨 실패 로그
		WSAGetLastError();

		closesocket(mh_listen_socket);
		WSACleanup();
	}
	ListenOverride();				// 프레임워크에 맞게 화면지정
}

void MyServer::AcceptClientConn()
{

}

void MyServer::RecvData()
{

}

void MyServer::SendData()
{

}

void MyServer::SendDataAllUsers()
{

}

// FD_CLOSE 또는 사용자 강퇴에 사용
// 특정 사용자의 접속 정보를 해제
void MyServer::CloseClientConn(SOCKET ah_socket, User* ap_close_user)
{
	User* p_user = GetUserIndex(ah_socket);
	if (p_user != NULL && ap_close_user != NULL) 
	{
		memcpy(ap_close_user, p_user, sizeof(User));			// ap_close_user에 퇴장 사용자정보 저장
		CloseClientOverride();
		p_user->h_socket = INVALID_SOCKET;
	}
	closesocket(ah_socket);
}

void MyServer::ClosetListen()
{
	if (mh_listen_socket != INVALID_SOCKET)
	{
		closesocket(mh_listen_socket);					// 리슨용 소켓 종료
		mh_listen_socket = INVALID_SOCKET;

		CloseListenOverride();
	}
}

void MyServer::CleanupWinSock()
{
	WSACleanup();

	CleanupOverride();
}

User* MyServer::GetUserIndex(SOCKET ah_socket)
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (m_user[i].h_socket == ah_socket) { return m_user + i; }
	}
	return NULL;				// 검색 실패시
}

