#ifndef LZR_SERIAL_RS485_H_
#define LZR_SERIAL_RS485_H_

#include <boost/asio.hpp>
//#include <boost/circular_buffer.hpp>
//#include <SafeQueue.h>

class CLzrCenterLogic;

class CLzrSerialRS485
{
public:
	explicit CLzrSerialRS485();
	virtual ~CLzrSerialRS485();
	CLzrSerialRS485(const CLzrSerialRS485&) = default;
	CLzrSerialRS485& operator=(const CLzrSerialRS485&) = default;
	CLzrSerialRS485(CLzrSerialRS485&&) noexcept = default;
	CLzrSerialRS485& operator=(CLzrSerialRS485&&) noexcept = default;


public:
	void setCenter(CLzrCenterLogic* _center);
	void start();
	void stop();
	void recvSerialMsg();
	void processRecvMsg();
	bool connSerialPort();
	void startReconnectTimer();
	void scheduleReconnect();

	void sendData(const std::vector<unsigned char>& data);								// 동기로 메시지 전송
	void asyncSendData(const std::vector<unsigned char>& data);						// 비동기로 메시지 전송

private:
	boost::asio::io_context m_io;
	boost::asio::serial_port m_serial;
	std::string m_strPort;
	std::unique_ptr<boost::asio::steady_timer> m_reconnectTimer;
	//boost::circular_buffer<unsigned char> m_recvRingBuf;
	long long m_reconnTimeSecond;

	std::vector<unsigned char> m_vecTempBuf;
	CSafeQueue<std::vector<unsigned char>> m_queueRecv;
	std::atomic_bool m_recvSerialFlag;
	unsigned int m_nSerialBaudRate;
	
	CLzrCenterLogic* m_pCenter;

};

#endif			LZR_SERIAL_RS485_H_