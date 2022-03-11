#include <stdio.h>

// PrintValue �Լ��� ���̺귯�� ����
// �Լ��������� �ƱԸ�Ʈ�� ���� ����ڰ� ������ �����ϰ� ����
void PrintVal(int a_value, void (*ap_modify)(int*))
{
	if (ap_modify != NULL) { (*ap_modify)(&a_value); }
	printf("Value = %d\n", a_value);
}

// Modify Value �Լ��� ���̺귯�� ����ڰ� �ʿ信 ���ؼ� ����
void ModifyAbsValue(int* ap_value)
{
	// ap_value �����Ͱ� ����Ű�� ����� ������ ����� �����Ѵ�
	if (*ap_value < 0) { *ap_value = *ap_value * (-1); }
}

void ModifyZeroValue(int* ap_value)
{
	// ap_value �����Ͱ� �⸮Ű�� ����� ������ 0���� �����Ѵ�.
	if (*ap_value < 0) { *ap_value = 0; }
}

int main94() {
	int value = -5;

	PrintVal(value, NULL);			// -5�� ���
	
	// PrintValue �Լ����� �Լ������Ϳ� ���� ModifyAbsValue �Լ��� ȣ��Ǿ� -5�� 5�� �����.
	PrintVal(value, ModifyAbsValue);

	// PrintValue �Լ����� �Լ������Ϳ� ���� ModifyZeroValue �Լ��� ȣ��Ǿ� -5�� 0���� ����
	PrintVal(value, ModifyZeroValue);

	return 0;
}
