#include "LzrStruct.h"
#include "LzrSerialRS485.h"
#include "LzrCenterLogic.h"
#include <iostream>
#include <string>
#include <vector>
#include <boost/locale.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

CLzrSerialRS485::CLzrSerialRS485()
	: m_pCenter(nullptr)
	, m_serial(m_io)
	, m_vecTempBuf(8192)
	//, m_recvRingBuf(4 * 1024 * 1024)				// 4MB
{
	m_nSerialBaudRate = 460800U;
#ifdef _WIN32
	m_strPort = "\\\\.\\COM9";
#else
	m_strPort = "/dev/ttyS0";			// 또는 /dev/ttyUSB0
#endif

	m_recvSerialFlag.store(true);
	std::thread thRecv(&CLzrSerialRS485::processRecvMsg, this);
	thRecv.detach();
}

CLzrSerialRS485::~CLzrSerialRS485()
{
	stop();
}

void CLzrSerialRS485::setCenter(CLzrCenterLogic* _center)
{
	m_pCenter = _center;
}

void CLzrSerialRS485::start()
{
	// 먼저 시리얼 포트를 열고 그 다음 해당 포트에 대해 수신을 시작한다
	bool conn_result = connSerialPort();
	if (conn_result == true)
	{
		recvSerialMsg();
	}
	else {
		startReconnectTimer();
	}

	m_io.run();
}

void CLzrSerialRS485::stop()
{
	m_recvSerialFlag.exchange(false);
	
	// 타이머 정지
	if (m_reconnectTimer) {
		m_reconnectTimer->cancel();
	}

	// 값 변경 완료
	if (m_serial.is_open()) {
		boost::system::error_code ec;
		m_serial.cancel(ec); // 비동기 작업 취소
		m_serial.close(ec);  // 포트 닫기
	}

	// io_context 정지
	m_io.stop();

	printf("[SERIAL] STOP\n");
}

void CLzrSerialRS485::recvSerialMsg()
{
	printf("recvSerialMsg 생성\n");
	m_serial.async_read_some(boost::asio::buffer(m_vecTempBuf), 
		[this](boost::system::error_code ec, std::size_t sizeLen)
		{
			if (!ec)
			{
				// 정상 수신
				std::vector<unsigned char> vecData(m_vecTempBuf.begin(), m_vecTempBuf.begin() + sizeLen);
				m_queueRecv.push(std::move(vecData));

				m_vecTempBuf.clear();
				recvSerialMsg();
			}
			else {
				std::cerr << "[SERIAL_RECV ERROR] " << ec.message() << std::endl;

				// 시리얼 오류 시 포트 닫고 재연결 시도
				if (m_serial.is_open()) {
					m_serial.close();
				}

				startReconnectTimer();
			}
		});
}

void CLzrSerialRS485::processRecvMsg()
{
	while (m_recvSerialFlag.load())
	{
		std::vector<unsigned char> recvData = m_queueRecv.wait_pop();

		if (m_pCenter)
		{
			m_pCenter->onRecvSerialMessage(recvData);
		}
	}
}

bool CLzrSerialRS485::connSerialPort()
{
	bool bResult = false;
	// 설정파일 정보를 통해 시리얼포트에 연결한다
	boost::property_tree::ptree m_ptree;
	try {
		boost::property_tree::ini_parser::read_ini("lzrconfig.ini", m_ptree);
		m_strPort = m_ptree.get<std::string>("serial.port");
	}
	catch (const boost::property_tree::ini_parser_error& e) {
		std::cerr << "[CONFIG ERROR]" << e.what() << std::endl;
	}

	try {
		if (m_serial.is_open()) {
			m_serial.close();
		}

		m_serial.open(m_strPort);
		m_serial.set_option(boost::asio::serial_port_base::baud_rate(m_nSerialBaudRate));

		bResult = true;
	}
	catch (const boost::system::system_error& e)
	{
		std::cerr << "[SERIAL ERROR] " << e.what() << std::endl;
	}

	return bResult;
}

void CLzrSerialRS485::startReconnectTimer()
{
	if (!m_reconnectTimer) {
		m_reconnectTimer = std::make_unique<boost::asio::steady_timer>(m_io);
		scheduleReconnect();
	}
}

void CLzrSerialRS485::scheduleReconnect()
{
	m_reconnectTimer->expires_after(std::chrono::seconds(3));				// 3초에 한번씩 재연결 시도

	m_reconnectTimer->async_wait([this](const boost::system::error_code& ec) {
		if (!ec && !m_serial.is_open()) {
			std::cerr << "[INFO] Attempting to reconnect serial..." << std::endl;
			try {
				m_serial.open(m_strPort);
				m_serial.set_option(boost::asio::serial_port_base::baud_rate(m_nSerialBaudRate));
				std::cout << "[SUCCESS] Serial reconnected." << std::endl;

				recvSerialMsg();  // 수신 재시작
			}
			catch (const boost::system::system_error& e) {
				std::cerr << "[RECONNECT ERROR] " << e.what() << std::endl;
				scheduleReconnect();  // 재시도 예약
			}
		}
		});
}

void CLzrSerialRS485::sendData(const std::vector<unsigned char>& data)
{
	boost::asio::write(m_serial, boost::asio::buffer(data));
}

void CLzrSerialRS485::asyncSendData(const std::vector<unsigned char>& data)
{
	boost::asio::async_write(m_serial, boost::asio::buffer(data), [](const boost::system::error_code& ec, std::size_t bytes_transferred) {
		if (ec) {
			std::cerr << "[ASYNC_SEND ERROR] " << ec.message() << std::endl;
		}
	});
}

