// Boost 라이브러리 1.83.0 버전
/*
설치
https://www.boost.org/ -> Download -> boost_1_83_0.zip -> 압축해제
boost_1_83_0 폴더 -> 관리자 권한으로 cmd 실행 -> .\bootstrap.bat 입력하여 실행
.\b2.exe 입력하여 빌드

포함 디렉토리 : C:\boost_1_83_0
라이브러리 디렉토리 : C:\boost_1_83_0\stage\lib
필요한 라이브러리를 입력에 넣어야함

*/
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00

#ifdef _DEBUG
#pragma comment(lib, "boost//libboost_system-vc142-mt-gd-x64-1_83.lib")
#else
#pragma comment(lib, "boost//libboost_system-vc142-mt-x64-1_83.lib")
#endif

#endif          // _WIN32

// C++20 코루틴
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/cancellation_signal.hpp>
#include <boost/asio/steady_timer.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <cstdio>
#include <chrono>





int main()
{

    return 0;
}
