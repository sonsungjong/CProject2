// �ߺ� ��Ŭ��带 ���� �뵵
#ifndef MYHEADER_H_
#define MY_HEADER_H_

// import <iostream>;
//#include <iostream>             // ���� ���Ͽ��� ����� �� �ְ� iostream ��� ���Ͽ� �ִ� ������ ��� ���������� ����
#include "HeaderUnits.h"

int main0059()
{
    // �ּ� : �����Ϸ��� �� �κ��� ����
    // ���� : ��ó�� -> ������ -> ��ũ
    printf("��ó�� : �ҽ� �ڵ忡 ��� ��Ÿ ������ ó��\n");
    printf("������ : �ҽ� �ڵ带 �ӽ��� ���� �� �ִ� ��ü ���Ϸ� ��ȯ\n");
    printf("��ũ : ��ü ������ ���ø����̼����� ���´�\n");

    std::cout << "There are " << 219 << " ways I love you." << std::endl;                   // ��� ��Ʈ��
    // std::cout << std::format("There are {} ways I love you.", 219) << std::endl;

    int value = 0;
    std::cin >> value;              // �Է� ��Ʈ��

    return 0;
}

#endif