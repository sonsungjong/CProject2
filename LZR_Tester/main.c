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
        printf("0. 종료\n1. COM번호 입력하고 연결하기\n2. 측정모드 전환(50001)\n3. 설정모드 전환(0xA5)\n4. 설정값보기(50004)\n5. 측정각도변경\n6. 정밀도변경\n");
        printf("7. 현재 모드 조회(50002)\n8. 레드 레이저 상태 조회(50010)\n");
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
            //request_getConfig();
        }
        else if (select_number == 5)
        {
            printf(">>5. 측정각도변경<<\n");
            int start_angle = -48;
            int end_angle = 48;
            printf("==> 시작 각도를 설정하세요(최저 -48):");
            scanf("%d", &start_angle);
            printf("==> 끝 각도를 설정하세요(최고 48):");
            scanf("%d", &end_angle);

            int distance_angle = end_angle - start_angle;
            //request_changeAngle(start_angle, end_angle, distance_angle);
        }
        else if (select_number == 6)
        {
            printf(">>6. 정밀도변경<<\n");
            //printf("==> 정밀도를 설정하세요:");
            //int point_level = 0;
            //scanf("%d", &point_level);

            //request_changePointLevel(point_level);
        }
        else if (select_number == 7)
        {
            printf(">>7. 현재 모드 조회<<\n");
            request_CurrentMode();
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

