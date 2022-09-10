#include <stdio.h>
#include <stdlib.h>				// atoi

int ArrayToInterger(char a_str[])
{
	int count = 0;
	int num = 0;
	while (a_str[count] != 0) {
		num = num * 10 + a_str[count] - '0';
		count++;
	}
	return num;
}

int main1406()
{
	int first_num = 0;
	int second_num = 0;
	char first_string[16];
	char second_string[16];

	printf("input first number >>");
	gets(first_string);
	printf("input second number >>");
	gets(second_string);

	first_num = atoi(first_string);													// 문자열 -> 정수
	second_num = ArrayToInterger(second_string);						// 문자열 -> 정수
	printf("%d + %d = %d\n", first_num, second_num, first_num + second_num);

	return 0;
}