#include <stdio.h>

int main166()
{
	int year = 2015, month = 12, day = 31;
	day++;

	if (day > 31) {
		month++;
		day = 1;
		if (month > 12) {
			year++;
			month = 1;
		}
	}
	printf("Date : %d�� %d�� %d��\n", year, month, day);

	return 0;
}