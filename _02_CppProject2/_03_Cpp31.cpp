#include <iostream>
// �����ε��� ���ӽ����̽�
extern "C"
{
	int add(int a, int b)
	{
		return a + b;
	}
}

namespace SSJ
{
	int add(int a, int b)
	{
		return a + b;
	}

	double add(double a, double b)
	{
		return a + b;
	}

	int add(int a, int b, int c)
	{
		return a + b + c;
	}
}

namespace myname
{
	int add(int a, int b)
	{
		return a + b;
	}

	double add(double a, double b)
	{
		return a + b;
	}

	int add(int a, int b, int c)
	{
		return a + b + c;
	}
}

using namespace myname;

int main31() {
	// namespace�� ������ �����ڿ� �Բ� ���
	int sum = SSJ::add(2, 3);
	// namespace�� �Ź� ���� ���� �����ϴٸ� using namespace�� ���
	int myname_sum = add(3.1, 4.1);

	return 0;
}

/*
	overloading : �Լ� �̸��� ���� ��Ʈ������ ����
	�����ڵ鰣 �Լ����� ��ġ�� ������ ����
	�������� �������� ������ ����� ��õ���� ����
	C������ �ȵ�
	�����ε��� ȿ���� �������� namespace�� �������

	namespace : C���� �Լ����� ��ġ�� �ȵǱ� ������ �� �Լ��տ� name�� �ٿ����µ� �ʹ� ������� namespace��� ������ ����
	namespace�� ������(::) �����ڿ� �Բ� ���
	namespace�� �Ź� ���� ���� �����ϴٸ� using namespace�� ���

	extern "C"{} �ȿ� �Լ��� �Ѱ� �־ C���� ȣȯ��ų �� ����
	�� ��� extern "C"{} �ȿ� �ִ� �Լ��� �⺻ �Լ��� ��
*/