#include "LzrStruct.h"
#include "LzrCenterLogic.h"
#include "LzrTCPServer.h"
#include "LzrSerialRS485.h"
#include <iostream>
#include <string>

CLzrCenterLogic::CLzrCenterLogic()
	: m_pSerial(nullptr)
	, m_pTCPServer(nullptr)
{

}

CLzrCenterLogic::~CLzrCenterLogic()
{

}


void CLzrCenterLogic::setAppServer(CLzrTCPServer* _tcp_server)
{
	m_pTCPServer = _tcp_server;
	_tcp_server->setCenter(this);
}

void CLzrCenterLogic::setLzrSerial(CLzrSerialRS485* _serial)
{
	m_pSerial = _serial;
	_serial->setCenter(this);
}

void CLzrCenterLogic::onRecvTCPMessage(const std::vector<unsigned char>& msg)
{
	// TCP로 받은 메시지를 해석한 후 처리

}

void CLzrCenterLogic::onRecvSerialMessage(const std::vector<unsigned char>& msg)
{
	// 센서에서 받은 메시지를 해석한 후 처리
	if (msg.empty() == false)
	{
		
	}
	else {
		printf("RecvSerialMessage is Empty!\n");
	}
}

void CLzrCenterLogic::onRecvCMDMessage(std::string input)
{
	// CMD로 입력
	if (input == "1") {

	}
	else if (input == "2") {

	}
	else if (input == "0")
	{
		if (m_pTCPServer) {
			m_pTCPServer->stop();
		}

		if (m_pSerial) {
			m_pSerial->stop();
		}
	}
	else {
		std::cout << "[USER INPUT] Not Defined Input Message\n";
	}
}
