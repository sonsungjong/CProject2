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
								//printf("[processThread] header_sync OK! CMD=%d, SIZE=%d, footer=%d\n", stData.message_cmd, stData.header_size, stData.footer_chk);
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

				// 타임아웃 설정
				COMMTIMEOUTS timeouts = { 10, 1, 10, 0, 0 };
				SetCommTimeouts(g_hSerial, &timeouts);

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
	}

	return nResult;
}

void closeSerialPort(void)
{
	// 종료 전 정리
	g_running_end = 1;
	Sleep(100);
	CloseHandle(g_hSerial);
	RingBuffer_destroy(&ring1);
	DeleteCriticalSection(&g_cs_end);
}

DWORD WINAPI sendThread(LPVOID lpParam)
{
	// 프레임 구성: SYNC(4) + SIZE(2) + CMD(2) + DATA(1) + CHK(2) = 11 bytes
	unsigned char packet[11] = { 0, };
	unsigned int sync = HEADER_SYNC_VAL;
	unsigned short size = 3;             // CMD(2) + DATA(1)
	unsigned short cmd = 50001;         // SETRAWDATAMODE
	unsigned char d0 = 1;             // 측정 모드 요청

	// CHK 계산
	unsigned short chk = 0;
	chk += cmd & 0xFF;        // LSB
	chk += (cmd >> 8);        // MSB
	chk += d0;

	// 패킷 작성 (LSB first)
	memcpy(&packet[0], &sync, 4);           // SYNC
	memcpy(&packet[4], &size, 2);           // SIZE
	memcpy(&packet[6], &cmd, 2);            // CMD
	packet[8] = d0;                         // DATA
	memcpy(&packet[9], &chk, 2);            // CHK

	DWORD bytesWritten = 0;
	WriteFile(g_hSerial, packet, sizeof(packet), &bytesWritten, NULL);

	if (bytesWritten == sizeof(packet))
	{
		printf("측정 모드 진입 명령 전송 완료 (0x5001)\n");
	}

	return 0;
}

void request_MeasurementMode(void)
{
	HANDLE hThread = (HANDLE)_beginthreadex(
		NULL,       // 보안 속성
		0,          // 스택 크기 (0 = 기본)
		sendThread, // 스레드 함수
		NULL,       // 파라미터
		0,          // 생성 후 바로 실행
		NULL
	);
	CloseHandle(hThread);
}

void request_ConfigMode(void)
{
	// 타이머를 생성해서 응답을 받기 전까지 계속 보낸다
	unsigned char packet = 0xA5;					// 패킷

	DWORD bytesWritten = 0;
	WriteFile(g_hSerial, &packet, sizeof(packet), &bytesWritten, NULL);
	if (bytesWritten == sizeof(packet)) {
		printf("설정 모드 진입 명령 전송 완료 (0xA5)\n");
	}
}

#endif