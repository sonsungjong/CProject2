#include <iostream>
using namespace std;

// Ŭ���̾�Ʈ�� ������ ����ϸ� ��(call)
// ������ Ŭ���̾�Ʈ�� ����ϸ� �ݹ�(callback)

void Client();				// ��ǻ� Ŭ���̾�Ʈ�� ������ �̸� �ƴ� ���� �Ұ���

// ����
void PrintHello()
{
	cout << "Hello!" << endl;
	Client();					// �������� Ŭ���̾�Ʈ�� �ڵ带 ȣ��
}

// Ŭ���̾�Ʈ
void Client()
{
	cout << "�� client" << endl;
}

int main067()
{
	PrintHello();					// ���� �ڵ� ȣ��

	return 0;
}