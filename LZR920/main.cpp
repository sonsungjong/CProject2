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

#include <iostream>
#include <deque>
#include <vector>
#include <tchar.h>
#include <memory>
#include <set>
#include <thread>
#include <optional>
//#include <boost/json.hpp>
//#include <boost/coroutine2/all.hpp>
//#include <SafeQueue.h>

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

#include "LzrDefine.h"
#include "LzrEnum.h"
#include "LzrStruct.h"
#include "LzrSerialRS485.h"
#include "LzrTCPServer.h"
#include "LzrCenterLogic.h"

int LZRMain()
{
    CLzrCenterLogic center;
    CLzrTCPServer tcp_server;
    CLzrSerialRS485 sensor;

    center.setLzrSerial(&sensor);
    center.setAppServer(&tcp_server);
    
    std::thread t1([&] {tcp_server.start(); });
    std::thread t2([&] {sensor.start(); });

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

