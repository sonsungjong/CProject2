#ifndef SERIAL_RS485_H_
#define SERIAL_RS485_H_

#ifdef __cplusplus
extern "C" {
#endif

#define HEADER_SYNC_VAL			0xFFFEFDFC

#define MIN_MSG_SIZE							8					// SYNC(4) + SIZE(2) + FOOTER(2)
#define HEADER_SYNC_SIZE					4
#define HEADER_MESSAGE_SIZE			2
#define MESSAGE_CMD_SIZE					2
#define FOOTER_CHK_SIZE					2

	typedef struct LZR920
	{
		unsigned int header_sync;					// 0xFFFEFDFC
		unsigned short header_size;				// message의 크기
		unsigned short message_cmd;				// 메시지 아이디
		unsigned char* message_data;			// 데이터
		unsigned short footer_chk;
	} ST_LZR920;

	int openSerialPort(char* portName, int baudRate);
	void closeSerialPort(void);


	// 메시지 생성부
	void request_MeasurementMode(void);
	void request_ConfigMode(void);
	void request_GetConfig(void);				// 현재 설정값 조회
	//void request_changeAngle(int start_angle, int end_angle, int distance_angle);
	void startTimerRequestConfigurationMode(void);			// 설정모드 요청 타이머
	void stopTimerRequestConfigurationMode(void);			// 설정모드 요청 타이머 중지
	void request_CurrentMode(void);			// 현재 모드 조회
	

#ifdef __cplusplus
}
#endif

#endif