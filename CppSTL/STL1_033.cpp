#include <iostream>
using namespace std;

// �Լ� ȣ�� ������ �����ε� : ��ü�� �Լ�ó�� ���۽�Ű�� ������
// �Լ� ��ü : �Լ� ȣ�� �������� ��ü

struct FuncObject
{
public:
	void operator() (int arg) const
	{
		cout << "���� : " << arg << endl;
	}
};

void Print1(int arg)
{
	cout << "���� : " << arg << endl;
}

int main033()
{
	void (*Print2)(int) = Print1;
	FuncObject Print3;

	Print1(10);						// �Լ����
	Print2(10);							// �Լ������� ���
	Print3(10);							// �Լ���ü ���
	//Print3.operator(10);

	return 0;
}