#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main_154()
{
	char greet[] = "Hello, Guys!";
	printf("1차원 배열 greet의 길이는 : %lld\n", sizeof(greet));

	char mind[30] = "I Love programming!";
	printf("%s\n", mind);

	mind[7] = '\0';						// 문자열은 null문자를 기준으로 종료를 인식함
	printf("%s \n", mind);

	mind[1] = '\0';
	printf("%s \n", mind);

	char str[50];
	printf("문자열을 입력하세요 >>");
	(void)scanf("%s", str);				// 문자열은 & 참조가 필요없음
	printf("입력된 문자열 : %s\n", str);

	return 0;
}