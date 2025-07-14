#ifndef LZR_TCP_SERVER_H_
#define LZR_TCP_SERVER_H_

#include <boost/asio.hpp>

//#include <boost/circular_buffer.hpp>
//#include <SafeQueue.h>

class CLzrCenterLogic;

class CLzrTCPServer
{
public:
	explicit CLzrTCPServer();
	virtual ~CLzrTCPServer();
	CLzrTCPServer(const CLzrTCPServer&) = default;
	CLzrTCPServer& operator=(const CLzrTCPServer&) = default;
	CLzrTCPServer(CLzrTCPServer&&) noexcept = default;
	CLzrTCPServer& operator=(CLzrTCPServer&&) noexcept = default;

public:
	void setCenter(CLzrCenterLogic* _center);
	void start();
	void stop();

private:
	boost::asio::io_context m_io;
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::asio::ip::tcp::socket m_socket;

	std::vector<unsigned char> m_vecTempBuf;
	CSafeQueue<std::vector<unsigned char>> m_queueRecv;
	std::atomic_bool m_recvTCPFlag;
	unsigned int m_nPort;

	CLzrCenterLogic* m_pCenter;

};

#endif		LZR_TCP_SERVER_H_