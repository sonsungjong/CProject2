#ifndef MY_SERVER_H_
#define MY_SERVER_H_

// 통신 + 문자열변환 + 파일로그(C:\\log\\server.txt)
// 소켓통신은 바이트 단위로 이루어지기 때문에 정수는 바이트로 직렬화해서 전송해야함
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
	User m_user[MAX_USER];			// 사용자정보
	SOCKET mh_listen_socket;			// 접속용 소켓
	int m_accept_msg_id;			// 비동기 접속 메시지
	int m_read_close_msg_id;			// 비동기 읽기/종료 메시지
	char m_msg[MSG_SIZE];				// 메시지구조
	User* mp_net_user;					// 데이터를 전송한 클라이언트의 정보
	User m_close_user_data;			// 나간 사람에 대한 정보 백업


public:
	explicit MyServer();				// 생성자
	virtual ~MyServer();				// 파괴자

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

	// 부가기능
	User* GetUserIndex(SOCKET ah_socket);				// 소켓핸들을 통해 m_user 배열의 저장된 항목 위치를 얻음
	inline User* GetUser() { return m_user; }			// 클라이언트 목록 시작주소 반환

	
	// 오버라이딩 전용 (개발환경에 맞게 후속처리)
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