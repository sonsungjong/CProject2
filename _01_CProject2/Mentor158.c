#include <stdio.h>

int main158()
{
	// #1
	int num[5] = { 1,2,3,4,5 };

	int total = 0;
	for (size_t i = 0; i < sizeof(num)/sizeof(int); i++)
	{
		total += num[i];
	}

	printf("총합은 %d\n", total);

	// #2
	char str[50];
	char u_str[50] = { 0, };
	printf("문자열을 입력하세요 >>");
	scanf_s("%s", str, (unsigned int)sizeof(str));

	int str_size = strlen(str);				// 5

	for (int i = 0; i < str_size; i++)
	{
		u_str[i] = str[str_size-i-1];
		// 0 4 : h
		// 1 3 : e
		// 2 2 : l
		// 3 1 : l
		// 4 0 : o
	}
	u_str[str_size] = '\0';
	printf("%s\n", u_str);

	// #3
	char eng[50];
	char ascii = 0;
	int i = 0;
	printf("영단어 입력 >>");
	scanf_s("%s", eng, (unsigned int)sizeof(eng));
	
	for(i = 0; i<sizeof(eng); i++)
	{
		if (eng[i] == '\0') {
			break;
		}
		if (ascii < eng[i]) {
			ascii = eng[i];
		}
	}

	printf("아스키코드값이 가장 큰 문자 : %c \n", ascii);

	return 0;
}