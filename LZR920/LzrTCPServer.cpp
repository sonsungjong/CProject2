#include "LzrStruct.h"
#include "LzrTCPServer.h"
#include "LzrCenterLogic.h"
#include <iostream>
#include <string>
#include <vector>
#include <boost/locale.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

CLzrTCPServer::CLzrTCPServer()
	: m_pCenter(nullptr)
	, m_vecTempBuf(8192)
	, m_io()
	, m_acceptor(m_io)
	, m_socket(m_io)
{
}

CLzrTCPServer::~CLzrTCPServer()
{
	
}


void CLzrTCPServer::setCenter(CLzrCenterLogic* _center)
{
	m_pCenter = _center;
}

void CLzrTCPServer::start()
{

	// ������ť ����
	m_recvTCPFlag.store(true);
}


void CLzrTCPServer::stop()
{
	// ������ť ���� �÷���
	if (m_recvTCPFlag.load() == true)
	{
		m_recvTCPFlag.store(false);
	}
}