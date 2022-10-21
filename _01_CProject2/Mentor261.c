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
	// 싱글 포인터
	int* ptr1 = &num;
	// 더블 포인터
	int** dptr = &ptr1;
	int* ptr2;

	printf("%p %p\n", ptr1, dptr);
	printf("%d %d\n", num, **dptr);
	ptr2 = *dptr;
	*ptr2 = 9;				// num = 9
	printf("%d %d\n", num, **dptr);

	// 더블포인터 참조
	int num1 = 66, num2 = 99;
	int* pptr1 = &num1;
	int* pptr2 = &num2;
	printf("*pptr1 : %d *pptr2 : %d \n", *pptr1, *pptr2);
	MenSwapNumber(&pptr1, &pptr2);
	printf("*pptr1 : %d *pptr2 : %d\n", *pptr1, *pptr2);

	// 삼중포인터
	int number = 999;
	int* ptr3 = &number;						// 변수 number의 주소값을 포인터에 저장
	int** dptr3 = &ptr3;						// 포인터변수 ptr3의 주소값을 이중포인터 dptr에 저장
	int*** tptr3 = &dptr3;					// 이중 포인터변수 dptr의 주소값을 삼중포인터 tptr에 저장

	printf("%d %d %d %d\n", number, *ptr3, **dptr3, ***tptr3);

	return 0;
}