#include <stdio.h>
// this �����Ϳ� ���ؼ�

// ����ü
struct MyData
{
	int a;
	int b;
};

// �Լ�
int Sum(struct MyData* ap_data)
{
	return ap_data->a + ap_data->b;
}

int main11()
{
	struct MyData data = { 5, -2 };
	int result;

	result = Sum(&data);
	printf("%d\n", result);

	return 0;
}

/*
	�Լ� : �ڵ��� ��ȭ�� ��ó
	����ü : �ڷ����� ��ȭ�� ��ó
	������ : �޸��� ����� ���̰��� ����󵵸� ����

	�ڷ��� == ��

	1. Ŭ����, this������, ���ٱ���������
	2. ������, �ı���, new, delete
	3. �����ε�, namespace, ������
	4. ���
*/