#include "pch.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "serial_rs485.h"

int main()
{
    while (1)
    {
        // 버튼 대용
        printf("0. 종료\n1. COM번호 입력하고 연결하기\n2. 측정모드 전환(50001)\n3. 설정모드 전환(0xA5)\n4. 설정값보기(50004)\n5. 설정 변경\n6. 변경 설정 저장\n");
        printf("7. 현재 모드 조회(50002)\n8. 셋팅값 공장 초기화(50007)\n9. 레드 레이저 상태 조회(50010)\n");
        int select_number = -1;
        scanf("%d", &select_number);

        if (select_number == 1)
        {
            printf(">>1. 연결하기<<\n");
            printf("COM 포트 번호를 입력하세요: ");
            int com_number = 0;
            scanf("%d", &com_number);

            char com_name[16] = { 0 };
            sprintf(com_name, "\\\\.\\COM%d", com_number);
            
            int nResult = openSerialPort(com_name, 460800);          // COM 연결을 시도하고 연결에 성공하면 수신쓰레드를 생성한다
            if (nResult == 1) {
                printf("%s 연결 성공\n", com_name);

                // 연결 성공하면 바로 현재 모드를 얻어온다
                request_CurrentMode();
            }
            else {
                printf("%s 연결 실패\n", com_name);
            }
        }
        else if (select_number == 2)
        {
            printf(">>2. 측정모드<<\n");
            request_MeasurementMode();
        }
        else if (select_number == 3)
        {
            printf(">>3. 설정모드<<\n");
            request_ConfigMode();
        }
        else if (select_number == 4)
        {
            printf(">>4. 설정값보기<<\n");
            request_GetConfig();          // 설정모드에서 1초마다 동작하게 자동 셋팅
        }
        else if (select_number == 5)
        {
            printf(">>5. 설정 변경<<\n");
            int start_spot = 0; int input_start = 0;            // [0]왼쪽 48도 ~ [273]오른쪽 48도
            int end_spot = 274; int input_end = 274;            // [1]왼쪽 처음 ~ [274]오른쪽 끝
            int apd_distance = 2;
            //printf("==> 시작점을 입력하세요 [0~273]");       // -48
            //scanf("%d", &input_start);
            //if (input_start >= 0 && input_start <= 273) {
            //    start_spot = input_start;           // D18~19
            //}
            //printf("==> 끝 점을 입력하세요 [1~274]:");      // 48
            //scanf("%d", &input_end);
            //if (input_end >= 1 && input_end <= 274 && input_end > input_start) {
            //    end_spot = input_end;               // D16~17
            //}

            //printf("==> 측정 거리를 입력하세요 [0=8m, 1=12m, 2=16m]:");
            //scanf("%d", &apd_distance);
            //if (apd_distance < 0 || apd_distance > 2) {
            //    apd_distance = 1;
            //}

            int distance_angle = end_spot - start_spot;
            ST_DataConfig stData;
            initConfigData(&stData);
            // 입력값 : plane0123 활성/비활성, 시작점위치[0~273], 끝점위치[1~274], 측정거리[0~2]
            stData.D1617_number_distance_values = (unsigned short)end_spot;
            stData.D1819_starting_spot = (unsigned short)start_spot;
            stData.D22_apd_distance_range = (unsigned char)apd_distance;

            request_changeSetting(stData);
        }
        else if (select_number == 6)
        {
            printf(">>6. 변경 설정 저장<<\n");

            request_saveConfig_EEPROM();
        }
        else if (select_number == 7)
        {
            printf(">>7. 현재 모드 조회<<\n");
            request_CurrentMode();
        }
        else if (select_number == 8)
        {
            printf(">>8. 셋팅값 공장 초기화<<\n");
            request_RestoreSetting();
        }
        else if (select_number == 9)
        {
            printf(">>9. 레드레이저 조회<<\n");
            //request_Red_Laser();
        }
        else if (select_number == 0)
        {
            closeSerialPort();
            break;
        }
    }


    printf("프로그램 정상 종료\n");
    return 0;
}

