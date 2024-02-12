// Boost 라이브러리 1.84.0 버전
/*
설치
https://www.boost.org/ -> Download -> boost_1_84_0.zip -> 압축해제
boost_1_84_0 폴더 -> 관리자 권한으로 cmd 실행 -> .\bootstrap.bat 입력하여 실행
.\b2.exe 입력하여 빌드

포함 디렉토리 : C:\boost_1_84_0
라이브러리 디렉토리 : C:\boost_1_84_0\stage\lib
필요한 라이브러리를 입력에 넣어야함

라이브러리 입력(릴리즈64) : libboost_context-vc143-mt-x64-1_84.lib
라이브러리 입력(릴리즈64) : libboost_coroutine-vc143-mt-x64-1_84.lib

라이브러리 입력(디버그64) : libboost_context-vc143-mt-gd-x64-1_84.lib
라이브러리 입력(디버그64) : libboost_coroutine-vc143-mt-gd-x64-1_84.lib

라이브러리 입력(릴리즈32) : libboost_context-vc143-mt-x32-1_84.lib
라이브러리 입력(릴리즈32) : libboost_coroutine-vc143-mt-x32-1_84.lib

라이브러리 입력(디버그32) : libboost_context-vc143-mt-gd-x32-1_84.lib
라이브러리 입력(디버그32) : libboost_coroutine-vc143-mt-gd-x32-1_84.lib
*/
#include <iostream>
#include <boost/coroutine2/all.hpp>

#ifdef _DEBUG
#pragma comment(lib, "libboost_context-vc143-mt-gd-x64-1_84.lib")
#pragma comment(lib, "libboost_coroutine-vc143-mt-gd-x64-1_84.lib")
#else
#pragma comment(lib, "libboost_context-vc143-mt-x64-1_84.lib")
#pragma comment(lib, "libboost_coroutine-vc143-mt-x64-1_84.lib")
#endif

void MyCooperativeFunc(boost::coroutines2::coroutine<void>::push_type& a_sink)
{
    printf("Hello ");
    a_sink();                       // 코루틴 코드 실행을 멈추고 메인 함수로 돌아감
    printf("World!\n");
    // 코루틴 중단
}

int main()
{
    boost::coroutines2::coroutine<void>::pull_type src(MyCooperativeFunc);              // 코루틴 생성
    printf("From ");
    src();                              // 코루틴 코드 실행을 재개
    printf(" Coroutine\n");

    return 0;
}
