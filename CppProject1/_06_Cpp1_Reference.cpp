#include <iostream>

using namespace std;

void SwapPointer(int* ap_a, int* ap_b)
{
	int temp;
	temp = *ap_a;
	*ap_a = *ap_b;
	*ap_b = temp;
}

void SwapReference(int& ap_a, int& ap_b)
{
	// ���α׷� ������ڵ��� ������ �����;��� ������ ������ �� ������ ���۷�������
	int temp;
	temp = ap_a;
	ap_a = ap_b;
	ap_b = temp;
	// ������ ��ȣ�� ���ܹ���
}

int mainReference() {
	int data = 5;
	int& r = data;		// ���۷��� : ������ ���ϰ� ����� ���� ����, �Ⱦ��� �� ����
	int* p = &data;	// ������

	cout << data << endl;
	r = 7;		// data = 7;
	cout << data << endl;
	*p = 8;		//	data = 8;
	cout << data << endl;

	int num1 = 3, num2 = 6;
	SwapPointer(&num1, &num2);
	SwapReference(num1, num2);
	cout << num1 << endl;
	cout << num2 << endl;

	// ����δ� ������ ������ (���⸸ �� ���ϰ� ��������)
	// ��, ��ȯ���� ���۷����� �ϸ� Ÿ�̹�(���̽������) ������ �߻���
	return 0;
}