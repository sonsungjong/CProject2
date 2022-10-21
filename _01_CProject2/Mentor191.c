#include <stdio.h>

int main_191()
{
	int arr[2][3] =
	{
		{1,2,3},
		{4,5,6}
	};

	int (*arrptr)[3] = arr;

	// 포인터 변수를 통한 배열 요소 출력
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%d ", arrptr[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	// #1
	char bingo[3][3];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == j)
			{
				bingo[i][j] = 'O';
			}
			else
			{
				bingo[i][j] = 'X';
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%c ", bingo[i][j]);
		}
		printf("\n");
	}

	// #2
	int gugu[4][9];

	for (int i = 2; i < 6; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			gugu[i-2][j-1] = i * j;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			printf("%d ", gugu[i][j]);
		}
		printf("\n");
	}

	// #3
	int arr3[3][2] =
	{
		{6,7},{8,9},{3,4}
	};
	printf("%d %d \n", arr3[1][0], arr3[0][1]);
	printf("%d %d \n", *(arr3[2]+1), *(arr3[1]+1));
	printf("%d %d \n", (*(arr3+2))[0], (*(arr3 + 0))[1]);
	printf("%d %d \n", **arr3, *(*(arr3+0)+0));

	return 0;
}