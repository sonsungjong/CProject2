#include <iostream>
using namespace std;
void Client();

// ����
void PrintHello() {
	cout << "Hello, Server Start!" << endl;
	Client();			// �ݹ� (�������� Ŭ���̾�Ʈ ȣ��)
}

// Ŭ���̾�Ʈ
void Client()
{
	cout << "I am Client" << endl;
}

int main236()
{
	PrintHello();			// ���� ȣ��

	return 0;
}

/*
	Ŭ���̾�Ʈ�� ���� ȣ�� : �� (Call)
	������ Ŭ���̾�Ʈ ȣ�� : �ݹ� (CallBack)
*/