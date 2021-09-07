#include <stdio.h>

struct MyData
{
	// �Ű������� �Ѿ �������� ����ü�� �̸� ��Ƴ��´�.
	int a;
	int b;
	void (*p_modify_a)(int*);
	void (*p_modify_b)(int*);
};

int add(struct MyData *ap_data)
{
	if (ap_data->p_modify_a)(*(ap_data->p_modify_a))(&ap_data->a);
	if (ap_data->p_modify_b)(*(ap_data->p_modify_b))(&ap_data->b);
	
	return ap_data->a + ap_data->b;
}

void ModifyData(int* ap_num)
{
	if (*ap_num < 0) *ap_num *= -1;
}

int main901() {
	struct MyData md;
	md.a = 2;
	md.b = -3;
	md.p_modify_a = ModifyData;
	md.p_modify_b = ModifyData;

	int s = add(&md);		// �Լ� �ƱԸ�Ʈ�� ��ȭ�� ���� ���ɼ��� ���� ��� ����� �ʵ� ���� ��ȭ�Ƚ�Ű���� ����ü���

	return 0;
}