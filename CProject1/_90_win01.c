#include <stdio.h>

struct MyData
{
	// 매개변수로 넘어갈 정보들을 구조체로 미리 잡아놓는다.
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

	int s = add(&md);		// 함수 아규먼트에 변화가 생길 가능성이 있을 경우 사용한 쪽도 같이 변화안시키려고 구조체사용

	return 0;
}