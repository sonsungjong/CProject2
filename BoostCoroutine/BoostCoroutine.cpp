// Boost ���̺귯�� 1.84.0 ����
/*
��ġ
https://www.boost.org/ -> Download -> boost_1_84_0.zip -> ��������
boost_1_84_0 ���� -> ������ �������� cmd ���� -> .\bootstrap.bat �Է��Ͽ� ����
.\b2.exe �Է��Ͽ� ����

���� ���丮 : C:\boost_1_84_0
���̺귯�� ���丮 : C:\boost_1_84_0\stage\lib
�ʿ��� ���̺귯���� �Է¿� �־����

���̺귯�� �Է�(������64) : libboost_context-vc143-mt-x64-1_84.lib
���̺귯�� �Է�(������64) : libboost_coroutine-vc143-mt-x64-1_84.lib

���̺귯�� �Է�(�����64) : libboost_context-vc143-mt-gd-x64-1_84.lib
���̺귯�� �Է�(�����64) : libboost_coroutine-vc143-mt-gd-x64-1_84.lib

���̺귯�� �Է�(������32) : libboost_context-vc143-mt-x32-1_84.lib
���̺귯�� �Է�(������32) : libboost_coroutine-vc143-mt-x32-1_84.lib

���̺귯�� �Է�(�����32) : libboost_context-vc143-mt-gd-x32-1_84.lib
���̺귯�� �Է�(�����32) : libboost_coroutine-vc143-mt-gd-x32-1_84.lib
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
    a_sink();                       // �ڷ�ƾ �ڵ� ������ ���߰� ���� �Լ��� ���ư�
    printf("World!\n");
    // �ڷ�ƾ �ߴ�
}

int main()
{
    boost::coroutines2::coroutine<void>::pull_type src(MyCooperativeFunc);              // �ڷ�ƾ ����
    printf("From ");
    src();                              // �ڷ�ƾ �ڵ� ������ �簳
    printf(" Coroutine\n");

    return 0;
}
