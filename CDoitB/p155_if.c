#include <stdio.h>

int main155()
{
	int score = 86;
	char grade = '\0';

	if (score >= 90)
	{
		grade = 'A';
	}
	else if (score >= 80) {
		grade = 'B';
	}
	else if (score >= 70) {
		grade = 'C';
	}
	else if (score >= 60) {
		grade = 'D';
	}
	else {
		grade = 'F';
	}

	printf("����� ������ %d���̰� ����� %c�Դϴ�. \n", score, grade);

	return 0;
}