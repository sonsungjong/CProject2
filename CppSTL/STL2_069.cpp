#include <iostream>
using namespace std;

// ������ Ŭ���̾�Ʈ�� �Լ��� �̸� �˰��ִ� ���� �Ұ�����
// �ݹ� ��Ŀ������ Ŭ���̾�Ʈ�� ������ ȣ���� �� ������ Ŭ���̾�Ʈ�� ������ �����ϵ��� ����
// �Լ� �������� �Ű������� �̿��� �ݹ� �Լ��� �ּҸ� ������

// ����
// �迭�� ��� ���ҿ� �ݺ����� �۾��� �����ϰ� �߻�ȭ�� (��ü���� �۾��� ����)
void For_each(int* begin, int* end, void (*pf)(int))
{
	while (begin != end)
	{
		pf(*begin++);				// Ŭ���̾�Ʈ �Լ� ȣ�� (�ݹ�)
	}
}

// Ŭ���̾�Ʈ1
void PrintClient1(int n)
{
	cout << n << ' ';				// ������ �̿��� �������
}

// Ŭ���̾�Ʈ2
void PrintClient2(int n)			// �� ���Ҹ� ������ ���
{
	cout << n * n << " ";
}

// Ŭ���̾�Ʈ3
void PrintClient3(int n)
{
	cout << "���� : " << n << endl;				// ���ڿ��� �ٹٲ��� �̿��� ���� ���
}

int main069()
{
	int arr[5] = { 10, 20, 30, 40, 50 };

	For_each(arr, arr + 5, PrintClient1);				// �Ű������� Print1() �� �ּҸ� ����
	cout << endl << endl;
	For_each(arr, arr + 5, PrintClient2);				// �Ű������� Print2() �Լ��� �ּҸ� ����
	cout << endl << endl;
	For_each(arr, arr + 5, PrintClient3);				// �Ű������� Print3() �Լ��� �ּҸ� ����

	return 0;
}