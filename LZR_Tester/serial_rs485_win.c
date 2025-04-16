#ifdef _WIN32
#include "serial_rs485.h"
#include <Windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "RingBuffer1.h"

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



// static으로 접근 범위를 이 파일로 한정시킨다
static HANDLE g_hSerial = INVALID_HANDLE_VALUE;
static const char* g_portName = "\\\\.\\COM9";
static const int g_baudRate = 460800;
static ST_RingBuffer ring1;
static HANDLE g_hRecvThread;
static HANDLE g_hProcessThread;

static CRITICAL_SECTION g_cs_end;
static CONDITION_VARIABLE g_cv_end;
static int g_running_end = 0;			// [0] 종료X [1] 종료O

// 수신부
static DWORD WINAPI recvThreadSerial(void* lpParam)
{
	while (g_running_end == 0)
	{
		unsigned int bytesRead = 0;
		unsigned char tempBuf[16384] = { 0, };
		// ReadFile 로 수신받는다
		if (ReadFile(g_hSerial, tempBuf, sizeof(tempBuf), &bytesRead, NULL))
		{
			if (bytesRead > 0)
			{
				RingBuffer_write(&ring1, (const unsigned char*)tempBuf, bytesRead);
			}
		}
	}

	return 0;
}

// 처리부
DWORD WINAPI processMessageThread(void* lpParam)
{
	while (g_running_end == 0)
	{
		unsigned char* data = NULL;
		size_t buf_size = RingBuffer_wait_read(&ring1, &data);
		if (data != NULL && buf_size > 0)
		{
			size_t offset = 0;

			while (buf_size >= 6)					// 최소 헤더
			{
				ST_LZR920 stData;
				size_t idx = 0;
				memcpy(&stData.header_sync, data + idx, sizeof(stData.header_sync));
				idx += sizeof(stData.header_sync);

				if (stData.header_sync == HEADER_SYNC_VAL)			// SYNC 검사 (FF FD FE FF)
				{
					memcpy(&stData.header_size, data + idx, sizeof(stData.header_size));
					idx += sizeof(stData.header_size);
					size_t total_len = sizeof(stData.header_sync) + sizeof(stData.header_size) + stData.header_size + sizeof(stData.footer_chk);

					if (total_len <= buf_size)
					{
						memcpy(&stData.message_cmd, data + idx, sizeof(stData.message_cmd));
						idx += sizeof(stData.message_cmd);

						size_t size_body_msg = stData.header_size - sizeof(stData.message_cmd);
						stData.message_data = (unsigned char*)malloc(size_body_msg);
						if (stData.message_data)
						{
							memcpy(stData.message_data, data + idx, size_body_msg);
							idx += size_body_msg;
							memcpy(&stData.footer_chk, data + idx, sizeof(stData.footer_chk));

							// CHK 검증
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
								printf("[processThread] header_sync OK! CMD=%d, SIZE=%d, footer=%d\n",
									stData.message_cmd,
									stData.header_size,
									stData.footer_chk);
							}

							if (stData.message_cmd == 50011)			// MDI
							{
								if (size_body_msg == 2202)
								{
									// ID + Frame counter
									// Plane Number + MDI

									// 앞에 6바이트를 다시 떼어내고
									// 그 다음 Plane Number 1바이트를 읽고
									// 그 다음 548 바이트를 unsigned short로 각각 변환한다음
									// 다시 Plane Number 1바이트를 읽고
									// 다시 548 바이트를 unsigned short로 각각 변환하고... 총 4번
									printf("MDI\n");
								}
								else if (size_body_msg == 2196)
								{
									// Plane Number + MDI
									printf("MDI\n");
								}
								else if (size_body_msg == 2216)
								{
									// ID + Frame counter
									// CTN + VNR + Error log + Hot reset counter
									// Plane Number + MDI
									printf("MDI\n");
								}
								else if (size_body_msg == 2210)
								{
									// CTN + VNR + Error log + Hot reset counter
									// Plane Number + MDI
									printf("MDI\n");
								}
							}
							else if (stData.message_cmd == 50002)			// 모드 변경 응답
							{
								unsigned char mode = stData.message_data[0];
								if (mode == 1)
								{
									printf("측정모드 응답\n");
								}
								else if (mode == 2)
								{
									printf("설정모드 응답\n");
								}
								else
								{
									printf("알수없는 모드\n");
								}
							}
						}
					}
				}
				else
				{
					// 헤더가 안맞으면 앞에서 1바이트 버리고 1바이트만 추가로 읽어온 후 다시 시도한다
					memmove(data, data + 1, buf_size - 1);
					buf_size--;
					unsigned char addByte = RingBuffer_readbyte(&ring1);
					data[buf_size] = addByte;
					buf_size++;
				}
			}

			free(data);
		}
	}
	return 0;
}


int openSerialPort(char* portName, int baudRate)
{
	int nResult = 0;
	InitializeCriticalSection(&g_cs_end);			// 큐 크리티컬 섹션 초기화

	if (portName)
	{
		g_hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		// 시리얼 파라미터 설정
		DCB dcbSerialParams;
		memset(&dcbSerialParams, 0, sizeof(dcbSerialParams));
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

		if (GetCommState(g_hSerial, &dcbSerialParams))
		{
			// 보레이트, 데이터 비트, 스톱 비트 등 설정 (예: 460800 8N1)
			dcbSerialParams.BaudRate = baudRate;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.Parity = NOPARITY;
			dcbSerialParams.StopBits = ONESTOPBIT;

			if (SetCommState(g_hSerial, &dcbSerialParams))
			{
				// 타임아웃 설정
				COMMTIMEOUTS timeouts = { 0 };
				timeouts.ReadIntervalTimeout = 50;
				timeouts.ReadTotalTimeoutConstant = 50;
				timeouts.ReadTotalTimeoutMultiplier = 10;
				timeouts.WriteTotalTimeoutConstant = 50;
				timeouts.WriteTotalTimeoutMultiplier = 10;

				if (SetCommTimeouts(g_hSerial, &timeouts))
				{
					if (g_hSerial != INVALID_HANDLE_VALUE)
					{
						// 링버퍼 생성
						int max_buffer = 1024 * 1024 * 2;           // 2MB
						int ring_id = 0;
						RingBuffer_init(&ring1, max_buffer, ring_id);

						// 처리 및 수신쓰레드 생성
						g_hProcessThread = CreateThread(NULL, 0, processMessageThread, NULL, 0, NULL);
						g_hRecvThread = CreateThread(NULL, 0, recvThreadSerial, NULL, 0, NULL);
						nResult = 1;
					}
				}
				else
				{
					printf("[openSerialPort] SetCommTimeouts() failed.\n");
					CloseHandle(g_hSerial);
					g_hSerial = INVALID_HANDLE_VALUE;
				}
			}
			else {
				printf("[openSerialPort] SetCommState() failed.\n");
				CloseHandle(g_hSerial);
				g_hSerial = INVALID_HANDLE_VALUE;
			}
		}
		else 
		{
			printf("[openSerialPort] GetCommState() failed.\n");
			CloseHandle(g_hSerial);
			g_hSerial = INVALID_HANDLE_VALUE;
		}
	}

	return nResult;
}

void closeSerialPort(void)
{
	// 종료 전 정리
	g_running_end = 1;
	Sleep(100);
	RingBuffer_destroy(&ring1);
	DeleteCriticalSection(&g_cs_end);
	CloseHandle(g_hSerial);
}

#endif