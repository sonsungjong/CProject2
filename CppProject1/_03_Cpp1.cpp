#include <iostream>
// using namespace : Ư���� namespace�� ������ std::�� ���ڴٴ� ��
using namespace std;

// �Լ� �����ε�
/*
int add(int a, int b) 
{
	return a + b;
}
*/

int add(int a, int b, int c)
{
	return a + b + c;
}

double add(double a, double b)
{
	return a + b;
}

// �����ε� ���ؼ� C�� ����
extern "C" {
	int add(int a, int b)
	{
		return a + b;
	}
}

// namespace (ssj_add���� ������ �տ� �̸����̸� �����غ����� ����)
namespace ssj
{
	// �� ���� ������, �Լ����� �տ� ssj�� ���� ���� ��
	int add(int a, int b)
	{
		return a + b;
	}
}

// using namespace�鳢�� ����,�Լ����� ��ġ�� �ȵ�
// using namespace ssj;

int main�Լ������ε�() {
	int s = add(2, 3);
	double temp = add(2.1, 3.0);
	int myName = ssj::add(3, 3);

	cout << "����: " << s << endl;
	cout << "�Ǽ�: " << temp << endl;
	cout << "namespace: " << myName << endl;

	return 0;
}

// �����ε��ϸ� C�� ȣȯ�� ��������
// extern "C"{} �ȿ� �־� ȣȯ