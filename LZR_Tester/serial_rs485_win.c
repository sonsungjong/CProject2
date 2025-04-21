#ifdef _WIN32
#include "serial_rs485.h"
#include <Windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <memory.h>
#include "RingBuffer1.h"
#include <mmsystem.h>			//#include <unistd.h>

#pragma comment(lib, "winmm.lib")

/*
	0xA5 : ���� ��� ���� [��]
	50001 : ���� ��� ���� [��]
	50002 : ��������ȸ(REQ) + ��� ���� ���� (RES) [��]
	50003 : ���� ���� [��]
	50004 : ������ ��ȸ(REQ) + ������ ���� (RES) [��]
	50005 : RAM������ EEPROM�� ����
	50006 : �α� �� ī���� �ʱ�ȭ
	50007 : RAM ���� �ʱ�ȭ
	50008 : �������� �ϰ� ��ȸ
	50009 : ������ 3�� ON/OFF
	50010 : ���� ������ ���� ��ȸ(REQ) + ����(RES)
	50011 : MDI ��ȯ
	50012 : ��Ʈ��Ʈ ON/OFF
	50013 : ��Ʈ��Ʈ ���� ��ȸ(REQ) + ����(RES)
	50014 : SETRAWDATAFRAMECOUNTERRESET
	50015 : X

*/

#define MODE_UNKNOWN				 0
#define MODE_MEASURE				 1
#define MODE_CONFIG					 2

#define MAX_CFG_SEND_COUNT			100			// ������� ��û �ִ� Ƚ��

// static���� ���� ������ �� ���Ϸ� ������Ų��
static HANDLE g_hSerial = INVALID_HANDLE_VALUE;
static const char* g_portName = "\\\\.\\COM9";
static const int g_baudRate = 460800;
static ST_RingBuffer ring1;
static HANDLE g_hRecvThread;
static HANDLE g_hProcessThread;

static UINT g_mmTimerID = 0;
static volatile LONG g_curMode = MODE_MEASURE;      // 1: ����, 2: ����, 0: �˼����� (�⺻���� �������� �س��� ����û�� �غ���)
static CRITICAL_SECTION g_txCS;

static CRITICAL_SECTION g_cs_end;
static CONDITION_VARIABLE g_cv_end;
static int g_running_end = 0;			// [0] ����X [1] ����O
static volatile LONG g_cfgSendCount = 0;      // ������� A5 ���� Ƚ�� ī��Ʈ


// ���ź�
static DWORD WINAPI recvThreadSerial(void* lpParam)
{

	while (g_running_end == 0)
	{
		unsigned int bytesRead = 0;
		unsigned char tempBuf[16384] = { 0, };
		// ReadFile �� ���Ź޴´�
		EnterCriticalSection(&g_txCS);
		BOOL read_result = ReadFile(g_hSerial, tempBuf, sizeof(tempBuf), &bytesRead, NULL);
		LeaveCriticalSection(&g_txCS);
		if (read_result)
		{
			if (bytesRead > 0)
			{
				RingBuffer_write(&ring1, (const unsigned char*)tempBuf, bytesRead);
			}
		}
	}

	return 0;
}

// ó����
DWORD WINAPI processMessageThread(void* lpParam)
{
	while (g_running_end == 0)
	{
		unsigned char* data = NULL;
		size_t buf_size = RingBuffer_wait_read(&ring1, &data);
		if (data != NULL && buf_size > 0)
		{
			while (buf_size >= 6)					// �ּ� ���
			{
				ST_LZR920 stData;
				size_t idx = 0;

				// 1. SYNC
				memcpy(&stData.header_sync, data + idx, sizeof(stData.header_sync));
				idx += sizeof(stData.header_sync);

				if (stData.header_sync != HEADER_SYNC_VAL)
				{
					// �߸��� SYNC�̸� 1����Ʈ ������ �絿��ȭ
					memmove(data, data + 1, buf_size - 1);
					buf_size--;
					unsigned char add = RingBuffer_readbyte(&ring1);
					data[buf_size] = add;
					continue;
				}

				if (stData.header_sync == HEADER_SYNC_VAL)			// SYNC �˻� (FF FD FE FF)
				{
					memcpy(&stData.header_size, data + idx, sizeof(stData.header_size));
					idx += sizeof(stData.header_size);
					size_t total_len = sizeof(stData.header_sync) + sizeof(stData.header_size) + stData.header_size + sizeof(stData.footer_chk);

					// 4) ��ü �������� ���� �� �� ������ �����и�ŭ �� �б�
					if (total_len > buf_size)
					{
						unsigned long long need = total_len - buf_size;
						unsigned char* extra = NULL;
						unsigned long long got = RingBuffer_readbytes(&ring1, &extra, need);
						if (got > 0)
						{
							data = (unsigned char*)realloc(data, buf_size + got);
							memcpy(data + buf_size, extra, got);
							buf_size += got;
						}
						free(extra);
						// �׷��� ���ڶ�� ���� Ż���ؼ� ���� ���ʿ� �ٽ� �õ�
						if (total_len > buf_size)
							break;
					}

					if (total_len <= buf_size)
					{
						memcpy(&stData.message_cmd, data + idx, sizeof(stData.message_cmd));
						idx += sizeof(stData.message_cmd);

						size_t size_body_msg = stData.header_size - sizeof(stData.message_cmd);
						stData.message_data = NULL;
						if (size_body_msg > 0)
						{
							stData.message_data = (unsigned char*)malloc(size_body_msg);
							memcpy(stData.message_data, data + idx, size_body_msg);
							idx += size_body_msg;
						}

						if (stData.message_data)
						{
							memcpy(&stData.footer_chk, data + idx, sizeof(stData.footer_chk));

							// CHK ����
							unsigned short calc_chk = 0;
							for (int j = 0; j < sizeof(stData.message_cmd); ++j)
							{
								calc_chk += ((unsigned char*)&stData.message_cmd)[j];
							}
							for (int i = 0; i < size_body_msg; ++i)
							{
								calc_chk += stData.message_data[i];
							}

							if (stData.footer_chk == calc_chk)
							{
								printf("[processThread] header_sync OK! CMD=%d, SIZE=%d, footer=%d\n", stData.message_cmd, stData.header_size, stData.footer_chk);
							}

							if (stData.message_cmd == 50011)			// MDI
							{
								// MDI ������ �������ϱ� �������� ��ȯ
								//if (InterlockedCompareExchange(&g_curMode, 0, 0) != MODE_MEASURE)
								//{
								//	InterlockedExchange(&g_curMode, MODE_MEASURE);
								//}

								if (size_body_msg == 2202)
								{
									// ID + Frame counter
									// Plane Number + MDI

									// �տ� 6����Ʈ�� �ٽ� �����
									// �� ���� Plane Number 1����Ʈ�� �а�
									// �� ���� 548 ����Ʈ�� unsigned short�� ���� ��ȯ�Ѵ���
									// �ٽ� Plane Number 1����Ʈ�� �а�
									// �ٽ� 548 ����Ʈ�� unsigned short�� ���� ��ȯ�ϰ�... �� 4��
									//printf("MDI\n");
								}
								else if (size_body_msg == 2196)
								{
									// Plane Number + MDI
									//printf("MDI\n");
								}
								else if (size_body_msg == 2216)
								{
									// ID + Frame counter
									// CTN + VNR + Error log + Hot reset counter
									// Plane Number + MDI
									//printf("MDI\n");
								}
								else if (size_body_msg == 2210)
								{
									// CTN + VNR + Error log + Hot reset counter
									// Plane Number + MDI
									//printf("MDI\n");
								}
								break;
							}
							else if (stData.message_cmd == 50004)			// GETRAWDATACONFIG ���� (������)
							{
								// 1) status bits(D0-D3) ����
								uint8_t* p = stData.message_data;
								uint32_t status_bits;
								memcpy(&status_bits, stData.message_data + 0, sizeof(status_bits));

								// 2) ��� ���� D4-D5
								uint16_t comm_load;
								memcpy(&comm_load, stData.message_data + 4, sizeof(comm_load));

								// 3) Baud rate ���� D6
								uint8_t baud_cfg;
								memcpy(&baud_cfg, stData.message_data + 6, sizeof(baud_cfg));
								const char* baud_strs[] = { "57600", "115200", "230400", "460800", "921600" };
								const char* baud_rate = (baud_cfg < 5 ? baud_strs[baud_cfg] : "Unknown");

								// 4) Number of distance values D10-D11
								uint16_t num_dist;
								memcpy(&num_dist, stData.message_data + 10, sizeof(num_dist));

								// 5) Starting spot number D12-D13
								uint16_t start_spot;
								memcpy(&start_spot, stData.message_data + 12, sizeof(start_spot));

								// 6) Gap between spots D14-D15
								uint16_t gap_spot;
								memcpy(&gap_spot, stData.message_data + 14, sizeof(gap_spot));

								// 7) Plane enable ���� (status_bits �ȿ� bit5..bit8)
								uint8_t plane0 = p[11];
								uint8_t plane1 = p[12];
								uint8_t plane2 = p[13];
								uint8_t plane3 = p[14];

								// ���
								printf("=== ���� ������ ===\n");
								printf("��ź���: %u%%\n", comm_load);
								printf("Baud rate: %s\n", baud_rate);
								printf("�Ÿ��� ����: %u\n", num_dist);
								printf("���� Spot: %u\n", start_spot);
								printf("Spot ����: %u\n", gap_spot);
								printf("Plane enable: P0=%s, P1=%s, P2=%s, P3=%s\n",
									plane0 ? "Y" : "N",
									plane1 ? "Y" : "N",
									plane2 ? "Y" : "N",
									plane3 ? "Y" : "N");
								break;
							}
							else if (stData.message_cmd == 50002)			// ��� ���� ����
							{
								unsigned char mode = stData.message_data[0];
								if (mode == 1)
								{
									printf("=============>���� ��� ����\n");
									if (InterlockedCompareExchange(&g_curMode, 0, 0) != MODE_MEASURE) {
										stopTimerRequestConfigurationMode();
										InterlockedExchange(&g_curMode, MODE_MEASURE);
									}
									break;
								}
								else if (mode == 2)
								{
									printf("=============>���� ��� ����\n");
									if (InterlockedCompareExchange(&g_curMode, 0, 0) != MODE_CONFIG) {
										stopTimerRequestConfigurationMode();
										InterlockedExchange(&g_curMode, MODE_CONFIG);
									}
									break;
								}
								else
								{
									printf("=============>�˼� ���� ���\n");
									InterlockedExchange(&g_curMode, MODE_UNKNOWN);
									break;
								}
							}
						}

						if (stData.message_data)
						{
							free(stData.message_data);
							stData.message_data = NULL;
						}
					}
					else
					{
						// ����� ������ ũ�Ⱑ �ȸ����� ���� �б�
						//memmove(data, data + 1, buf_size - 1);
						//buf_size--;
						//unsigned char addByte = RingBuffer_readbytes(&ring1, );
						//data[buf_size] = addByte;
						//buf_size++;
						break;
					}
				}
				else
				{
					// ����� �ȸ����� �տ��� 1����Ʈ ������ 1����Ʈ�� �߰��� �о�� �� �ٽ� �õ��Ѵ�
					memmove(data, data + 1, buf_size - 1);
					buf_size--;
					unsigned char addByte = RingBuffer_readbyte(&ring1);
					data[buf_size] = addByte;
					buf_size++;
				}
			}
			if (data)
			{
				free(data);
				data = NULL;
			}
		}
	}
	return 0;
}


int openSerialPort(char* portName, int baudRate)
{
	int nResult = 0;
	InitializeCriticalSection(&g_cs_end);			// ť ũ��Ƽ�� ���� �ʱ�ȭ
	InitializeCriticalSection(&g_txCS);

	if (portName)
	{
		g_hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

		if (g_hSerial != INVALID_HANDLE_VALUE)
		{
			DCB dcb = { 0 };
			dcb.DCBlength = sizeof(DCB);
			BOOL state = GetCommState(g_hSerial, &dcb);
			if (TRUE == state)
			{
				dcb.BaudRate = baudRate;
				dcb.ByteSize = 8;
				dcb.Parity = NOPARITY;
				dcb.StopBits = ONESTOPBIT;
				SetCommState(g_hSerial, &dcb);

				// Ÿ�Ӿƿ� ����
				COMMTIMEOUTS timeouts = {
					MAXDWORD,  // ReadIntervalTimeout: ������ ����ŷ ���͹� ���
					0,         // ReadTotalTimeoutMultiplier
					18,         // ReadTotalTimeoutConstant: ù ����Ʈ�� �ִ� 15ms ��� (11ms ~ 19ms)
					0,         // WriteTotalTimeoutMultiplier
					0          // WriteTotalTimeoutConstant
				};
				SetCommTimeouts(g_hSerial, &timeouts);

				// ������ ����
				int max_buffer = 1024 * 1024 * 4;           // 4MB
				int ring_id = 0;
				RingBuffer_init(&ring1, max_buffer, ring_id);

				// ó�� �� ���ž����� ����
				g_hProcessThread = CreateThread(NULL, 0, processMessageThread, NULL, 0, NULL);
				g_hRecvThread = CreateThread(NULL, 0, recvThreadSerial, NULL, 0, NULL);
				request_GetConfig();
				nResult = 1;
			}
		}
	}

	return nResult;
}

void closeSerialPort(void)
{
	// ���� �� ����
	g_running_end = 1;
	Sleep(100);
	CloseHandle(g_hSerial);
	RingBuffer_destroy(&ring1);
	DeleteCriticalSection(&g_txCS);
	DeleteCriticalSection(&g_cs_end);
}

// ���� ��� ��û
DWORD WINAPI sendMeasurementThread(LPVOID lpParam)
{
	// ������ ����: SYNC(4) + SIZE(2) + CMD(2) + DATA(1) + CHK(2) = 11 bytes
	for (int i = 0; i < 3; i++) {
		unsigned char packet[11] = { 0, };
		unsigned int sync = HEADER_SYNC_VAL;
		unsigned short size = 3;             // CMD(2) + DATA(1)
		unsigned short cmd = 50001;         // SETRAWDATAMODE
		unsigned char d0 = 1;             // ���� ��� ��û

		// CHK ���
		unsigned short chk = 0;
		chk += cmd & 0xFF;        // LSB
		chk += (cmd >> 8);        // MSB
		chk += d0;

		// ��Ŷ �ۼ� (LSB first)
		memcpy(&packet[0], &sync, 4);           // SYNC
		memcpy(&packet[4], &size, 2);           // SIZE
		memcpy(&packet[6], &cmd, 2);            // CMD
		packet[8] = d0;                         // DATA
		memcpy(&packet[9], &chk, 2);            // CHK

		DWORD bytesWritten = 0;
		EnterCriticalSection(&g_txCS);
		WriteFile(g_hSerial, packet, sizeof(packet), &bytesWritten, NULL);
		LeaveCriticalSection(&g_txCS);

		if (bytesWritten == sizeof(packet))
		{
			printf("���� ��� ���� ��� ���� �Ϸ� (0x5001)\n");
		}
	}

	return 0;
}

// ��������
void request_MeasurementMode(void)
{
	// ������尡 �ƴҶ��� ������� ��û ����
	if (InterlockedCompareExchange(&g_curMode, 0, 0) != MODE_MEASURE)
	{
		HANDLE hThread = (HANDLE)_beginthreadex(
			NULL,       // ���� �Ӽ�
			0,          // ���� ũ�� (0 = �⺻)
			sendMeasurementThread, // ������ �Լ�
			NULL,       // �Ķ����
			0,          // ���� �� �ٷ� ����
			NULL
		);
		CloseHandle(hThread);
	}
	else {
		printf("�̹� ���� ���\n");
	}
}

// ���� ��� ��ȸ ��û
DWORD WINAPI sendGetModeThread(LPVOID lpParam)
{
	// ������ ����: SYNC(4) + SIZE(2) + CMD(2) + CHK(2) = 10 bytes
	for (int i = 0; i < 3; i++) {
		unsigned char packet[10];
		uint32_t sync = HEADER_SYNC_VAL;           // 0xFFFEFDFC
		uint16_t size = sizeof(uint16_t);         // CMD�� �����Ƿ� 2
		uint16_t cmd = 50002;                    // GETRAWDATAMODE
		uint16_t chk = (cmd & 0xFF) + (cmd >> 8); // üũ�� = CMD LSB+MSB

		// LSB first
		memcpy(&packet[0], &sync, 4);
		memcpy(&packet[4], &size, 2);
		memcpy(&packet[6], &cmd, 2);
		memcpy(&packet[8], &chk, 2);

		DWORD bytesWritten = 0;
		EnterCriticalSection(&g_txCS);
		WriteFile(g_hSerial, packet, sizeof(packet), &bytesWritten, NULL);
		LeaveCriticalSection(&g_txCS);

		if (bytesWritten == sizeof(packet)) {
			printf("���� ��� ��ȸ ��û ���� �Ϸ� (CMD=50002)\n");
		}
	}

	return 0;
}

// ȣ��� ����
void request_CurrentMode(void)
{
	HANDLE hThread = (HANDLE)_beginthreadex(
		NULL, 0,
		sendGetModeThread,
		NULL, 0, NULL
	);
	CloseHandle(hThread);
}


// ���� ����
void request_ConfigMode(void)
{
	// Ÿ�̸Ӹ� �����ؼ� ������ �ޱ� ������ ��� ������

	// ���� ��������
	if (InterlockedCompareExchange(&g_curMode, 0, 0) == MODE_MEASURE)
	{
		if (g_mmTimerID == 0)
		{
			startTimerRequestConfigurationMode();
		}
		else
		{
			printf("Ÿ�̸� �̹� ���� �� �Դϴ�.\n");
		}
	}
	else
	{
		printf("�̹� ���� ��� �Դϴ�\n");
	}
}

// ������� ��û Ÿ�̸�
static void CALLBACK TimerCallbackProc_REQ_CONFIGUREMODE(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	// �ִ� Ƚ�� �ʰ� �� �ڵ� ����
	if (InterlockedIncrement(&g_cfgSendCount) > MAX_CFG_SEND_COUNT)
	{
		stopTimerRequestConfigurationMode();
		printf("���� ��� ���� ��� ������ %dȸ ���� �� �ߴ��߽��ϴ�.\n", MAX_CFG_SEND_COUNT);
	}
	else
	{
		if (InterlockedCompareExchange(&g_curMode, 0, 0) == MODE_CONFIG)
		{
			// ��������
			stopTimerRequestConfigurationMode();
		}
		else
		{
			// ���� ��������
			const unsigned char a5 = 0xA5;
			DWORD bytesWritten = 0;

			EnterCriticalSection(&g_txCS);
			WriteFile(g_hSerial, &a5, 1, &bytesWritten, NULL);
			LeaveCriticalSection(&g_txCS);
		}
	}
}

void startTimerRequestConfigurationMode(void)
{
	if (g_mmTimerID == 0)
	{
		InterlockedExchange(&g_cfgSendCount, 0);					// ���� ī���� �ʱ�ȭ(100ȸ����)
		unsigned int interval = 50;			// 50ms
		timeBeginPeriod(1);

		g_mmTimerID = timeSetEvent(interval, 0, TimerCallbackProc_REQ_CONFIGUREMODE, 0, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
	}
}

void stopTimerRequestConfigurationMode(void)
{
	if (g_mmTimerID != 0)
	{
		timeKillEvent(g_mmTimerID);
		g_mmTimerID = 0;
		timeEndPeriod(1);
	}
}

// GETRAWDATACONFIG ��û ������ ���� ������
DWORD WINAPI sendGetConfigThread(LPVOID lpParam)
{
	// SYNC(4) + SIZE(2) + CMD(2) + CHK(2) = 10 bytes
	while (g_running_end == 0)
	{
		if (InterlockedCompareExchange(&g_curMode, 0, 0) == MODE_CONFIG)
		{
			unsigned char packet[10];
			uint32_t sync = HEADER_SYNC_VAL;      // 0xFFFEFDFC
			uint16_t size = sizeof(uint16_t);    // CMD�� �����Ƿ� 2
			uint16_t cmd = 50004;               // GETRAWDATACONFIG
			uint16_t chk = (cmd & 0xFF) + (cmd >> 8); // üũ�� = LSB+MSB

			memcpy(&packet[0], &sync, 4);
			memcpy(&packet[4], &size, 2);
			memcpy(&packet[6], &cmd, 2);
			memcpy(&packet[8], &chk, 2);

			DWORD bytesWritten = 0;
			EnterCriticalSection(&g_txCS);
			WriteFile(g_hSerial, packet, sizeof(packet), &bytesWritten, NULL);
			LeaveCriticalSection(&g_txCS);

			if (bytesWritten == sizeof(packet))
			{
				printf("������ ��ȸ ��û ���� �Ϸ� (CMD=50004)\n");
			}
			Sleep(1000);
		}
		else {
			Sleep(1000);
		}
	}
	return 0;
}

// �ܺο��� ȣ���� ����
void request_GetConfig(void)
{
	// ����/���� ���� ������� ������ ��û ����
	HANDLE hThread = (HANDLE)_beginthreadex(
		NULL, 0,
		sendGetConfigThread,
		NULL, 0, NULL
	);
	CloseHandle(hThread);
}

#endif