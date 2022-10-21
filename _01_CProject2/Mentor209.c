#include <stdio.h>
#include <string.h>

int main_209()
{
	int is_same = -1;
	char str1[50];
	char str2[50];

	// scanf 띄어쓰기 입력받기 "%[^\n]"
	printf("첫번째 문자열>>");
	scanf_s("%[^\n]", str1, 50);
	rewind(stdin);										// 입력버퍼 비우기
	printf("두번째 문자열>>");
	rewind(stdin);
	scanf_s("%[^\n]", str2, 50);

	if (strcmp(str1, str2) == 0)
	{
		printf("같습니다\n");
	}
	else {
		printf("다릅니다\n");
	}

	int str1_length = strlen(str1);
	int str2_length = strlen(str2);

	if (str1_length >= str2_length)
	{
		printf("%s\n", str1);
		printf("%s\n", str2);
	}
	else {
		printf("%s\n", str2);
		printf("%s\n", str1);
	}

	int total = 0;
	for (int i = 0; i < str1_length; i++)
	{
		switch (str1[i])
		{
		case 48:
			total += 0;
			break;
		case 49:
			total += 1;
			break;
		case 50:
			total += 2;
			break;
		case 51:
			total += 3;
			break;
		case 52:
			total += 4;
			break;
		case 53:
			total += 5;
			break;
		case 54:
			total += 6;
			break;
		case 55:
			total += 7;
			break;
		case 56:
			total += 8;
			break;
		case 57:
			total += 9;
			break;
		default:
		{}
		}
	}
	printf("숫자의 합은 %d\n", total);

	return 0;
}