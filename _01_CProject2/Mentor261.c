#include <stdio.h>

void MenSwapNumber(int** dptr1, int** dptr2)
{
	int* temp = *dptr1;
	*dptr1 = *dptr2;
	*dptr2 = temp;
}

int main_261()
{
	int num = 3;
	// �̱� ������
	int* ptr1 = &num;
	// ���� ������
	int** dptr = &ptr1;
	int* ptr2;

	printf("%p %p\n", ptr1, dptr);
	printf("%d %d\n", num, **dptr);
	ptr2 = *dptr;
	*ptr2 = 9;				// num = 9
	printf("%d %d\n", num, **dptr);

	// ���������� ����
	int num1 = 66, num2 = 99;
	int* pptr1 = &num1;
	int* pptr2 = &num2;
	printf("*pptr1 : %d *pptr2 : %d \n", *pptr1, *pptr2);
	MenSwapNumber(&pptr1, &pptr2);
	printf("*pptr1 : %d *pptr2 : %d\n", *pptr1, *pptr2);

	// ����������
	int number = 999;
	int* ptr3 = &number;						// ���� number�� �ּҰ��� �����Ϳ� ����
	int** dptr3 = &ptr3;						// �����ͺ��� ptr3�� �ּҰ��� ���������� dptr�� ����
	int*** tptr3 = &dptr3;					// ���� �����ͺ��� dptr�� �ּҰ��� ���������� tptr�� ����

	printf("%d %d %d %d\n", number, *ptr3, **dptr3, ***tptr3);

	return 0;
}