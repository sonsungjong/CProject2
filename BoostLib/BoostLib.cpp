// Boost 라이브러리 1.83.0 버전
/*
설치
https://www.boost.org/ -> Download -> boost_1_83_0.zip -> 압축해제
boost_1_83_0 폴더 -> 관리자 권한으로 cmd 실행 -> .\bootstrap.bat 입력하여 실행
.\b2.exe 입력하여 빌드

포함 디렉토리 : boost_1_83_0
라이브러리 디렉토리 : boost_1_83_0\stage\lib
필요한 라이브러리를 입력에 넣어야함

라이브러리 입력(릴리즈64) : libboost_thread-vc143-mt-x64-1_83.lib
라이브러리 입력(디버그64) : libboost_thread-vc143-mt-gd-x64-1_83.lib
라이브러리 입력(릴리즈32) : libboost_thread-vc143-mt-x32-1_83.lib
라이브러리 입력(디버그32) : libboost_thread-vc143-mt-gd-x32-1_83.lib
*/

#include <cstdio>
#include <tchar.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <boost/random.hpp>

// 로그
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/async_frontend.hpp>


namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

#ifdef _DEBUG
#pragma comment(lib, "boost//libboost_random-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_log-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_log_setup-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_thread-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_system-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_filesystem-vc143-mt-gd-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_date_time-vc143-mt-gd-x64-1_88.lib")
#else
#pragma comment(lib, "boost//libboost_random-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_log-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_log_setup-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_thread-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_system-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_filesystem-vc143-mt-x64-1_88.lib")
#pragma comment(lib, "boost//libboost_date_time-vc143-mt-x64-1_88.lib")
#endif

void init_log()
{
    namespace logging = boost::log;
    namespace sinks = boost::log::sinks;
    namespace keywords = boost::log::keywords;

    // 파일 백엔드 생성
    using backend_t = sinks::text_file_backend;
    auto backend = boost::make_shared<backend_t>(
        keywords::file_name = "async_%Y%m%d_%H%M%S.log",
        keywords::rotation_size = 5 * 1024 * 1024
    );

    // 비동기 싱크 생성
    using sink_t = sinks::asynchronous_sink<backend_t>;
    auto sink = boost::make_shared<sink_t>(backend);

    // 포맷 설정 (선택)
    sink->set_formatter(
        logging::expressions::stream
        << "[" << logging::expressions::format_date_time<boost::posix_time::ptime>(
            "TimeStamp", "%Y-%m-%d %H:%M:%S")
        << "] [" << logging::trivial::severity << "] "
        << logging::expressions::smessage
    );

    // 싱크 등록
    logging::core::get()->add_sink(sink);
    logging::add_common_attributes();
}

int main()
{
    printf("%d\n", _MSC_VER);             // visual studio 버전 확인

    

    return 0;
}

