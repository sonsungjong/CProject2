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
		unsigned short header_size;				// message�� ũ��
		unsigned short message_cmd;				// �޽��� ���̵�
		unsigned char* message_data;			// ������
		unsigned short footer_chk;
	} ST_LZR920;

	int openSerialPort(char* portName, int baudRate);
	void closeSerialPort(void);


	// �޽��� ������
	void request_MeasurementMode(void);
	void request_ConfigMode(void);
	void request_GetConfig(void);				// ���� ������ ��ȸ
	//void request_changeAngle(int start_angle, int end_angle, int distance_angle);
	void startTimerRequestConfigurationMode(void);			// ������� ��û Ÿ�̸�
	void stopTimerRequestConfigurationMode(void);			// ������� ��û Ÿ�̸� ����
	void request_CurrentMode(void);			// ���� ��� ��ȸ
	

#ifdef __cplusplus
}
#endif

#endif