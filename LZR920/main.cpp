/*
Boost 라이브러리 1.88.0 버전 설치
https://www.boost.org/ -> Download -> boost_1_88_0.zip -> 압축해제
boost_1_88_0 폴더 -> 관리자 권한으로 cmd 실행 -> .\bootstrap.bat 입력하여 실행
.\b2.exe 입력하여 빌드

포함 디렉토리 : C:\boost_1_88_0
라이브러리 디렉토리 : C:\boost_1_88_0\stage\lib
필요한 라이브러리를 입력에 넣어야함

라이브러리 입력(릴리즈64) : libboost_system-vc143-mt-x64-1_88.lib
라이브러리 입력(디버그64) : libboost_system-vc143-mt-gd-x64-1_88.lib
라이브러리 입력(릴리즈32) : libboost_system-vc143-mt-x32-1_88.lib
라이브러리 입력(디버그32) : libboost_system-vc143-mt-gd-x32-1_88.lib

라이브러리 입력(릴리즈64) : libboost_json-vc143-mt-x64-1_88.lib
라이브러리 입력(디버그64) : libboost_json-vc143-mt-gd-x64-1_88.lib
라이브러리 입력(릴리즈32) : libboost_json-vc143-mt-x32-1_88.lib
라이브러리 입력(디버그32) : libboost_json-vc143-mt-gd-x32-1_88.lib
*/

#include "LzrStruct.h"

#ifdef _DEBUG
#pragma comment(lib, "libboost_system-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "libboost_locale-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "libboost_container-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "libboost_json-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "libboost_context-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "libboost_coroutine-vc143-mt-gd-x64-1_88.lib")
#else
#pragma comment(lib, "libboost_system-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "libboost_locale-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "libboost_container-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "libboost_json-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "libboost_context-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "libboost_coroutine-vc143-mt-x64-1_88.lib")
#endif


#include "LzrSerialRS485.h"
#include "LzrTCPServer.h"
#include "LzrCenterLogic.h"
#include "LogManager.h"

int LZRMain()
{
    setlocale(LC_ALL, "");
    CLogManager::getInstance().log("로그 기록 테스트");

    CLzrCenterLogic center;
    CLzrTCPServer tcp_server;
    CLzrSerialRS485 sensor;

    center.setLzrSerial(&sensor);
    center.setAppServer(&tcp_server);
    
    // TCP서버 관리 스레드
    std::thread t1([&] {
            try {
                tcp_server.start();
            }
            catch (const std::exception& e) {
                std::cerr << "[TCP_SERVER_THREAD ERROR] " << e.what() << std::endl;
            }
        }
    );

    // 센서 관리 스레드
    std::thread t2([&] {
            try {
                sensor.start();
            }
            catch (const std::exception& e) {
                std::cerr << "[SENSOR_THREAD ERROR] " << e.what() << std::endl;
            }
        }
    );

    // 메인스레드에서 사용자 입력 받기
    bool bEnd = false;
    char input_buf[256] = { 0 };
    while (bEnd != true) 
    {
        printf("0. 종료\n1. COM번호 입력하고 연결하기\n2. 측정모드 전환(50001)\n3. 설정모드 전환(0xA5)\n4. 설정값보기(50004)\n5. 설정 변경(50003)\n6. 변경 설정 저장(50005)\n");
        printf("7. 현재 모드 조회(50002)\n8. 셋팅값 공장 초기화(50007)\n9. 레드 레이저 상태 조회(50010)\n");
        memset(input_buf, 0, sizeof(input_buf));
        (void)scanf("%255[^\n]s", input_buf);
        getchar();

        printf("%s\n", input_buf);

        if(std::string(input_buf) != "0")
        {
            // center에 메시지 전달
            center.onRecvCMDMessage(std::string(input_buf));
        }
        else {
            // 프로그램 종료 ("0")
            bEnd = true;
            center.onRecvCMDMessage(std::string(input_buf));
        }
    }

    t1.join();
    t2.join();

    return 0;
}


int main()
{
    int nResult = 0;
    try {
        nResult = LZRMain();
    }
    catch (std::exception& e) {
        printf("%s\n", e.what());
    }

    return nResult;
}

