#include <cstdio>
#include <tchar.h>
#include <boost/random.hpp>

// Boost ���̺귯�� 1.84.0 ����
/*
��ġ
https://www.boost.org/ -> Download -> boost_1_84_0.zip -> ��������
boost_1_84_0 ���� -> ������ �������� cmd ���� -> .\bootstrap.bat �Է��Ͽ� ����
.\b2.exe �Է��Ͽ� ����

���� ���丮 : boost_1_84_0
���̺귯�� ���丮 : boost_1_84_0\stage\lib
�ʿ��� ���̺귯���� �Է¿� �־����

���̺귯�� �Է�(������64) : libboost_thread-vc143-mt-x64-1_84.lib
���̺귯�� �Է�(�����64) : libboost_thread-vc143-mt-gd-x64-1_84.lib
���̺귯�� �Է�(������32) : libboost_thread-vc143-mt-x32-1_84.lib
���̺귯�� �Է�(�����32) : libboost_thread-vc143-mt-gd-x32-1_84.lib
*/

int main()
{
    printf("%d\n", _MSC_VER);             // visual studio ���� Ȯ��

    // boost ����
    std::time_t now = std::time(0);
    boost::random::mt19937 gen{ static_cast<std::uint32_t>(now) };
    std::cout << gen() << '\n';
}

