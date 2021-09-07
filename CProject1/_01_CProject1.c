#include <stdio.h>

int main_1(void)
{
	
	short score;
	char grade[11] = {'F', 'F', 'F', 'F', 'F', 'F', 'D', 'C', 'B', 'A', 'A'};		// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10

	scanf_s("%hd", &score);
	score = score / 10;

	printf("grade : %c\n", grade[score]);
	
	return 0;
}