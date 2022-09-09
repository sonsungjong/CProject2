#include <stdio.h>

int main0802()
{
	/* 2015년 12월 31일을 3개의 정수 변수에 나누어서 대입함*/
	int year = 2016;
	int month = 5;
	int day = 31;

	/*일을 하루 증가시키면 day가 31에서 32가 됨*/
	day++;

	/* 일이 31일을 초과하면 1일로 바꾸고 월을 증가시킴*/
	if (day > 31) {
		/*월을 증가시키면 month가 12에서 13이됨*/
		month++;
		/*월이 증가되었으니 해당 월의 처음인 1일로 변경함*/
		day = 1;
		if (month > 12) {
			year++;
			month = 1;
		}
	}
	printf("Date : %d년 %d월 %d일\n", year, month, day);

	return 0;
}