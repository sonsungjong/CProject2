// Boost ���̺귯�� 1.83.0 ����
/*
��ġ
https://www.boost.org/ -> Download -> boost_1_83_0.zip -> ��������
boost_1_83_0 ���� -> ������ �������� cmd ���� -> .\bootstrap.bat �Է��Ͽ� ����
.\b2.exe �Է��Ͽ� ����

���� ���丮 : boost_1_83_0
���̺귯�� ���丮 : boost_1_83_0\stage\lib
�ʿ��� ���̺귯���� �Է¿� �־����

���̺귯�� �Է�(������64) : libboost_thread-vc143-mt-x64-1_83.lib
���̺귯�� �Է�(�����64) : libboost_thread-vc143-mt-gd-x64-1_83.lib
���̺귯�� �Է�(������32) : libboost_thread-vc143-mt-x32-1_83.lib
���̺귯�� �Է�(�����32) : libboost_thread-vc143-mt-gd-x32-1_83.lib
*/

#include <cstdio>
#include <tchar.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <boost/random.hpp>

// �α�
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

    // ���� �鿣�� ����
    using backend_t = sinks::text_file_backend;
    auto backend = boost::make_shared<backend_t>(
        keywords::file_name = "async_%Y%m%d_%H%M%S.log",
        keywords::rotation_size = 5 * 1024 * 1024
    );

    // �񵿱� ��ũ ����
    using sink_t = sinks::asynchronous_sink<backend_t>;
    auto sink = boost::make_shared<sink_t>(backend);

    // ���� ���� (����)
    sink->set_formatter(
        logging::expressions::stream
        << "[" << logging::expressions::format_date_time<boost::posix_time::ptime>(
            "TimeStamp", "%Y-%m-%d %H:%M:%S")
        << "] [" << logging::trivial::severity << "] "
        << logging::expressions::smessage
    );

    // ��ũ ���
    logging::core::get()->add_sink(sink);
    logging::add_common_attributes();
}

int main()
{
    printf("%d\n", _MSC_VER);             // visual studio ���� Ȯ��

    

    return 0;
}

