#include <stdio.h>
#include <string.h>

int main()
{
	int is_same = -1;
	char str1[50];
	char str2[50];

	printf("첫번째 문자열>>");
	scanf_s("%s", str1, 50);
	printf("두번째 문자열>>");
	scanf_s("%s", str2, 50);

	if (strcmp(str1, str2) == 0)
	{
		printf("같습니다\n");
	}
	else {
		printf("다릅니다\n");
	}

	return 0;
}