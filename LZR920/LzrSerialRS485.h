#ifndef LZR_SERIAL_RS485_H_
#define LZR_SERIAL_RS485_H_

#include <boost/asio.hpp>
//#include <boost/circular_buffer.hpp>
#include <SafeQueue.h>

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
	void recvSerialMsg();
	void processRecvMsg();
	void connSerialPort();

private:
	boost::asio::io_context m_io;
	boost::asio::serial_port m_serial;
	std::string m_strPort;
	//boost::circular_buffer<unsigned char> m_recvRingBuf;
	std::vector<unsigned char> m_vecTempBuf;
	CSafeQueue<std::vector<unsigned char>> m_queueRecv;
	std::atomic_bool m_recvFlag;
	unsigned int m_nSerialBaudRate;
	
	CLzrCenterLogic* m_pCenter;

};

#endif			LZR_SERIAL_RS485_H_