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
		// ���� ���̺귯�� �ʱ�ȭ ���� �α�

	}

	// Ŭ���̾�Ʈ ����Ʈ �ʱ�ȭ
	for (int i = 0; i < MAX_USER; ++i)
	{
		m_user[i].h_socket = INVALID_SOCKET;			// -1
	}

	mh_listen_socket = INVALID_SOCKET;				// ������ ���� �ʱ�ȭ
	memset(m_msg, 0, MSG_SIZE);
	mp_net_user = NULL;

	InitOverride();				// �����ӿ�ũ�� �°� �ʱ�ȭ�۾�
}

void MyServer::CreateSocket()
{
	mh_listen_socket = socket(AF_INET, SOCK_STREAM, 0);				// ���� ����
	if (mh_listen_socket == INVALID_SOCKET)
	{
		// ���� ���� ���� �α�
		WSAGetLastError();

		WSACleanup();
	}
	CreateOverride();				// �����ӿ�ũ�� �°� ������ ����� ȭ��� �޽�����ȣ�� ����
}

void MyServer::BindSocket(const TCHAR* a_ipv4, int a_port)
{
	sockaddr_in server_addr = { AF_INET, htons(a_port), };
	InetPton(AF_INET, a_ipv4, &server_addr.sin_addr.s_addr);

	int result = bind(mh_listen_socket, (sockaddr*)&server_addr, sizeof(server_addr));
	if (result == SOCKET_ERROR)
	{
		// ���ε� ���� �α�
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
		// ���� ���� �α�
		WSAGetLastError();

		closesocket(mh_listen_socket);
		WSACleanup();
	}
	ListenOverride();				// �����ӿ�ũ�� �°� ȭ������
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

// FD_CLOSE �Ǵ� ����� ���� ���
// Ư�� ������� ���� ������ ����
void MyServer::CloseClientConn(SOCKET ah_socket, User* ap_close_user)
{
	User* p_user = GetUserIndex(ah_socket);
	if (p_user != NULL && ap_close_user != NULL) 
	{
		memcpy(ap_close_user, p_user, sizeof(User));			// ap_close_user�� ���� ��������� ����
		CloseClientOverride();
		p_user->h_socket = INVALID_SOCKET;
	}
	closesocket(ah_socket);
}

void MyServer::ClosetListen()
{
	if (mh_listen_socket != INVALID_SOCKET)
	{
		closesocket(mh_listen_socket);					// ������ ���� ����
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
	return NULL;				// �˻� ���н�
}

