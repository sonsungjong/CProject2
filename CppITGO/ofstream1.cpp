// ���� ���� ���� (RAII ���� ����)

#include <iostream>
#include <fstream>

int main01()
{
    // ������ ���� ���ؼ��� ofstream ��ü�� �̿��Ѵ�
    //std::ofstream os("C:\\Data\\number.txt");       // ������
    std::ofstream os("number.txt");                     // �����

    // ����ó��
    if (!os) {
        std::cerr << "���� ���� ����\n";
        exit(1);
    }

    // 0 ~ 99 ���� ���Ͽ� ����
    for (int i = 0; i < 100; ++i)
    {
        os << i << " ";
    }

    // ��������� close���� �ʾƵ� �Ҹ��ڰ� ȣ��Ǿ� ���ҽ� ������

    return 0;
}
