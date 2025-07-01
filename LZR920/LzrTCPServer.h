#ifndef LZR_TCP_SERVER_H_
#define LZR_TCP_SERVER_H_

#include <boost/asio.hpp>
#include <boost/locale.hpp>
#include <boost/circular_buffer.hpp>

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

private:
	boost::asio::io_context m_io;

	CLzrCenterLogic* m_pCenter;

};

#endif		LZR_TCP_SERVER_H_