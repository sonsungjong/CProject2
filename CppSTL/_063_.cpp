#include <iostream>
using namespace std;

// �����Լ� : �����Լ�, namespace���� �����Լ�, static����Լ�
// �����Լ� ȣ�� : �ñ׳�ó�� ���� ���

void GPrint(int n)
{
	cout << "���� �Լ� : " << n << endl;
}

namespace A
{
	void Print(int n)
	{
		cout << "namespace A ���� �Լ� : " << n << endl;
	}
}

class Point
{
public:
	static void Print(int n)
	{
		cout << "Point Ŭ������ ���� ��� �Լ� : " << n << endl;
	}
};

int main063()
{
	void (*pf)(int);				// ���� �Լ� ������ ����

	GPrint(10);				// 1. namespace ���� �����Լ� ȣ��
	A::Print(10);					// 2. namespace A�� �����Լ� ȣ��
	Point::Print(10);						// 3. Point Ŭ������ ���� ����Լ� ȣ��

	pf = GPrint;					// 1. �Լ� �����ͷ� namespace ���� �����Լ� ����
	pf(10);

	pf = A::Print;						// 2. �Լ� �����ͷ� namespace�� A�� �����Լ� ����
	pf(10);

	pf = Point::Print;						// 3. �Լ� �����ͷ� PointŬ������ ���� ����Լ� ����
	pf(10);

	return 0;
}