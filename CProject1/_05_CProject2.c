#include <stdio.h>

void SwapVoidPointer(void* ap_a, void* ap_b);

int main52() {

	// void* : �������, Ÿ���� ������������ ������
	int data;
	short* p;

	// &data : int*
	p = (short*)&data;		// ����ȯ�� �ʿ�
	*p = 2;			// ������

	int data2;
	void* p2;

	p2 = &data2;			// ���������� �ʱ⶧���� ��� �ڷ����� �ּҸ� ���� �� ����
	*(short*)p2 = 7;	// ��� ����� �� ����ȯ �������

	// �Լ��� ���� ������ ��� �Ű������� void* �� ����� ����ϴ� �ʿ��� �ڷ����� ���� �� �ְ�����
	SwapVoidPointer(&data, &data2);
	
	return 0;
}

// �Ű������� void*�� ����
void SwapVoidPointer(void* ap_a, void* ap_b) {
	int temp;

	temp = *(int*)ap_a;
	*(int*)ap_a = *(int*)ap_b;
	*(int*)ap_b = temp;
}