#include <cstdio>
#include <tchar.h>
#include <boost/random.hpp>

// Boost 라이브러리 1.84.0 버전
/*
설치
https://www.boost.org/ -> Download -> boost_1_84_0.zip -> 압축해제
boost_1_84_0 폴더 -> 관리자 권한으로 cmd 실행 -> .\bootstrap.bat 입력하여 실행
.\b2.exe 입력하여 빌드

포함 디렉토리 : boost_1_84_0
라이브러리 디렉토리 : boost_1_84_0\stage\lib
필요한 라이브러리를 입력에 넣어야함

라이브러리 입력(릴리즈64) : libboost_thread-vc143-mt-x64-1_84.lib
라이브러리 입력(디버그64) : libboost_thread-vc143-mt-gd-x64-1_84.lib
라이브러리 입력(릴리즈32) : libboost_thread-vc143-mt-x32-1_84.lib
라이브러리 입력(디버그32) : libboost_thread-vc143-mt-gd-x32-1_84.lib
*/

int main()
{
    printf("%d\n", _MSC_VER);             // visual studio 버전 확인

    // boost 랜덤
    std::time_t now = std::time(0);
    boost::random::mt19937 gen{ static_cast<std::uint32_t>(now) };
    std::cout << gen() << '\n';
}

