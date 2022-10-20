#include <stdio.h>

int main_163()
{
	char cnum = 5;
	int inum = 999;
	double dnum = 3.14;

	char* cptr = &cnum;
	int* iptr = &inum;
	double* dptr = &dnum;

	printf("%p\n", cptr);
	printf("%p\n", iptr);
	printf("%p\n", dptr);

	int num = 10;
	int *pnum;
	pnum = &num;

	*pnum = 20;
	printf("num : %d\n", num);
	(*pnum)++;
	(*pnum)++;
	printf("num : %d\n", num);
	printf("*pnum : %d\n", *pnum);

	// # 1
	int a = 10;
	int* p = &a;
	*p = 20;
	printf("%d \n", a);				// 20
	*p = 50;
	printf("%d \n", a);					// 50

	// #2
	int number = 10;
	int *pnumber = &number;
	printf("%p \n", pnumber);
	printf("%d \n", *pnumber);

	// #3
	int* ptr;
	int num1 = 5;
	int num2 = 8;

	ptr = &num1;
	printf("%d\n", *ptr);

	ptr = &num2;
	printf("%d\n", *ptr);

	// #4
	int n1 = 100;
	int n2 = 200;
	int* ptr1 = &n1;
	int* ptr2 = &n2;
	printf("%d %d \n", *ptr1, *ptr2);

	int* ptr3 = ptr1;

	ptr1 = ptr2;
	ptr2 = ptr3;

	printf("%d %d \n", *ptr1, *ptr2);

	return 0;
}