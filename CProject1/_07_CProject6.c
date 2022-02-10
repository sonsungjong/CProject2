#include <stdio.h>
#include <malloc.h>

void Test(int** ap_data)
{
	**ap_data = (int**)malloc(sizeof(int) * 5);
}

int main76() {
	int* p;
	Test(&p);				// 포인터변수의 주소를 넘겨야 해제 가능

	free(p);

	return 0;
}