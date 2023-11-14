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

	printf("당신의 점수는 %d점이고 등급은 %c입니다. \n", score, grade);

	return 0;
}