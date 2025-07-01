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

void CLzrCenterLogic::onTCPMessage(const std::vector<unsigned char>& msg)
{
	// TCP로 받은 메시지를 해석해서 적절한 곳으로 보낸다

}

void CLzrCenterLogic::onSerialMessage(const std::vector<unsigned char>& msg)
{
	// 센서에서 받은 메시지를 해석해서 적절한 곳으로 보낸다

}
