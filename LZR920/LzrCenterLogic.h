#ifndef LZR_CENTER_LOGIC_H_
#define LZR_CENTER_LOGIC_H_

class CLzrTCPServer;
class CLzrSerialRS485;

#include <vector>

class CLzrCenterLogic
{
public:
	explicit CLzrCenterLogic();
	virtual ~CLzrCenterLogic();
	CLzrCenterLogic(const CLzrCenterLogic&) = default;
	CLzrCenterLogic& operator=(const CLzrCenterLogic&) = default;
	CLzrCenterLogic(CLzrCenterLogic&&) noexcept = default;
	CLzrCenterLogic& operator=(CLzrCenterLogic&&) noexcept = default;

public:
	void setAppServer(CLzrTCPServer* _tcp_server);
	void setLzrSerial(CLzrSerialRS485* _serial);

	void onTCPMessage(const std::vector<unsigned char>& msg);
	void onSerialMessage(const std::vector<unsigned char>& msg);

private:
	CLzrTCPServer* m_pTCPServer;
	CLzrSerialRS485* m_pSerial;


};

#endif			LZR_CENTER_LOGIC_H_