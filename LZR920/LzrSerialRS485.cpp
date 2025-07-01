#include "LzrSerialRS485.h"
#include "LzrCenterLogic.h"
#include <iostream>
#include <string>
#include <vector>
#include <boost/locale.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

CLzrSerialRS485::CLzrSerialRS485()
	: m_serial(m_io)
	, m_pCenter(nullptr)
	, m_vecTempBuf(8192)
	//, m_recvRingBuf(4 * 1024 * 1024)				// 4MB
{
	m_nSerialBaudRate = 460800U;
#ifdef _WIN32
	m_strPort = "\\\\.\\COM9";
#else
	m_strPort = "/dev/ttyS0";			// 또는 /dev/ttyUSB0
#endif

	m_recvFlag.store(true);
	std::thread thRecv(&CLzrSerialRS485::processRecvMsg, this);
	thRecv.detach();
}

CLzrSerialRS485::~CLzrSerialRS485()
{
	m_recvFlag.store(false);
}

void CLzrSerialRS485::setCenter(CLzrCenterLogic* _center)
{
	m_pCenter = _center;
}

void CLzrSerialRS485::start()
{
	connSerialPort();

	recvSerialMsg();
	m_io.run();
}

void CLzrSerialRS485::recvSerialMsg()
{
	m_serial.async_read_some(boost::asio::buffer(m_vecTempBuf), 
		[this](boost::system::error_code ec, std::size_t sizeLen)
		{
			if (!ec)
			{
				std::vector<unsigned char> vecData(m_vecTempBuf.begin(), m_vecTempBuf.begin() + sizeLen);
				m_queueRecv.push(std::move(vecData));

				m_vecTempBuf.clear();
				recvSerialMsg();
			}
		});
}

void CLzrSerialRS485::processRecvMsg()
{
	while (m_recvFlag.load())
	{
		std::vector<unsigned char> recvData = m_queueRecv.wait_pop();

		if (m_pCenter)
		{
			m_pCenter->onSerialMessage(recvData);
		}
	}
}

void CLzrSerialRS485::connSerialPort()
{
	// 설정파일 정보를 통해 시리얼포트에 연결한다
	boost::property_tree::ptree m_ptree;
	try {
		boost::property_tree::ini_parser::read_ini("lzrconfig.ini", m_ptree);
		m_strPort = m_ptree.get<std::string>("serial.port");
	}
	catch (const boost::property_tree::ini_parser_error& e) {
		std::cerr << "[CONFIG ERROR]" << e.what() << std::endl;
	}

	m_serial.open(m_strPort);
	m_serial.set_option(boost::asio::serial_port_base::baud_rate(m_nSerialBaudRate));
}

