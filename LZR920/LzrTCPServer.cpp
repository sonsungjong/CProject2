#include "LzrTCPServer.h"
#include "LzrCenterLogic.h"
#include <iostream>
#include <SafeQueue.h>
#include <string>
#include <vector>

CLzrTCPServer::CLzrTCPServer()
	: m_pCenter(nullptr)
	
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

}