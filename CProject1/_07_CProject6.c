#include <stdio.h>
#include <malloc.h>

void Test(int** ap_data)
{
	**ap_data = (int**)malloc(sizeof(int) * 5);
}

int main76() {
	int* p;
	Test(&p);				// �����ͺ����� �ּҸ� �Ѱܾ� ���� ����

	free(p);

	return 0;
}