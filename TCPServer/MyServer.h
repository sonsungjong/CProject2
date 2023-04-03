#ifndef MY_SERVER_H_
#define MY_SERVER_H_

// ��� + ���ڿ���ȯ + ���Ϸα�(C:\\log\\server.txt)
// ��������� ����Ʈ ������ �̷������ ������ ������ ����Ʈ�� ����ȭ�ؼ� �����ؾ���
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib, "WS2_32.lib")

#define MAX_USER				2000
#define MSG_SIZE			512

struct User {
	SOCKET h_socket;
	TCHAR ipv4[16];
};

// WSAStartup -> socket -> bind -> listen -> accept -> recv -> send -> closesocket(accept_socket) -> closesocket(listen_socket) -> WSACleanup()
class MyServer
{
private:
	User m_user[MAX_USER];			// ���������
	SOCKET mh_listen_socket;			// ���ӿ� ����
	int m_accept_msg_id;			// �񵿱� ���� �޽���
	int m_read_close_msg_id;			// �񵿱� �б�/���� �޽���
	char m_msg[MSG_SIZE];				// �޽�������
	User* mp_net_user;					// �����͸� ������ Ŭ���̾�Ʈ�� ����
	User m_close_user_data;			// ���� ����� ���� ���� ���


public:
	explicit MyServer();				// ������
	virtual ~MyServer();				// �ı���

	void InitWinSock();
	void CreateSocket();
	void BindSocket(const TCHAR* a_ipv4, int a_port);
	void StartListen();
	void AcceptClientConn();
	void RecvData();
	void SendData();
	void SendDataAllUsers();
	void CloseClientConn(SOCKET ah_socket, User* ap_close_user);
	void ClosetListen();
	void CleanupWinSock();

	// �ΰ����
	User* GetUserIndex(SOCKET ah_socket);				// �����ڵ��� ���� m_user �迭�� ����� �׸� ��ġ�� ����
	inline User* GetUser() { return m_user; }			// Ŭ���̾�Ʈ ��� �����ּ� ��ȯ

	
	// �������̵� ���� (����ȯ�濡 �°� �ļ�ó��)
	virtual void InitOverride() {}
	virtual void CreateOverride() {}
	virtual void BindOverride() {}
	virtual void ListenOverride() {}
	virtual void AcceptOverride() {}
	virtual void RecvOverride() {}
	virtual void SendOverride() {}
	virtual void SendAllOverride() {}
	virtual void CloseClientOverride() {}
	virtual void CloseListenOverride() {}
	virtual void CleanupOverride() {}
};

#endif