#include <stdio.h>

// 포인터 배열
int main_181()
{
	int n1 = 3, n2 = 6, n3 = 9;

	// 포인터변수의 선언 및 초기화
	int* ptr1 = &n1;
	int* ptr2 = &n2;
	int* ptr3 = &n3;

	int* parr[3] = { ptr1, ptr2, ptr3 };

	printf("%d %d %d \n", *parr[0], *parr[1], *parr[2]);
	printf("%p %p %p \n", parr[0], parr[1], parr[2]);

	// #1
	int arr[5] = { 10, 20, 30, 40, 50 };
	int* p_arr = arr;

	for (int i = 0; i < sizeof(arr)/sizeof(int); i++)
	{
		printf("%d ", *(p_arr+i));
	}
	printf("\n");

	// #2
	int arr2[8] = { 1,2,3,4,5,6,7,8 };
	int* p_arr2 = arr2 + 7;
	int total2 = 0;

	for (int i = 0; i < 8; i++)
	{
		int num = *(p_arr2 - i);
		if (num % 2 != 0) {
			total2 += num;
		}
	}

	printf("%d \n", total2);

	// #3
	int arr3[8] = { 1,2,3,4,5,6,7,8 };
	int* sp_arr = arr3;
	int* up_arr = arr3 + 7;
	int backup = 0;

	for (int i = 0; i < 4; i++)
	{
		backup = *(sp_arr + i);
		*(sp_arr+i) = *(up_arr-i);
		*(up_arr - i) = backup;
	}

	for (int i = 0; i < 8; i++) {
		printf("%d ", arr3[i]);
	}

	return 0;
}