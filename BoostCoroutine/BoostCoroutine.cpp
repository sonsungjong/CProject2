// Boost ���̺귯�� 1.83.0 ����
/*
��ġ
https://www.boost.org/ -> Download -> boost_1_83_0.zip -> ��������
boost_1_83_0 ���� -> ������ �������� cmd ���� -> .\bootstrap.bat �Է��Ͽ� ����
.\b2.exe �Է��Ͽ� ����

���� ���丮 : C:\boost_1_83_0
���̺귯�� ���丮 : C:\boost_1_83_0\stage\lib
�ʿ��� ���̺귯���� �Է¿� �־����

*/
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00

#ifdef _DEBUG
#pragma comment(lib, "boost//libboost_system-vc142-mt-gd-x64-1_83.lib")
#else
#pragma comment(lib, "boost//libboost_system-vc142-mt-x64-1_83.lib")
#endif

#endif          // _WIN32

// C++20 �ڷ�ƾ
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
