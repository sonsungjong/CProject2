#include <stdio.h>

int main175()
{
	int score = 86;
	char grade = '\0';

	switch (score / 10)
	{
	case 10:
	case 9:
		grade = 'A';
		break;
	case 8:
		grade = 'B';
		break;
	case 7:
		grade = 'C';
		break;
	case 6:
		grade = 'D';
		break;
	default:
		grade = 'F';
		break;
	}
	printf("����� ������ %d���̰� ����� %c�Դϴ� \n", score, grade);

	return 0;
}
