#include <stdio.h>
#include <string.h>

int main()
{
	int is_same = -1;
	char str1[50];
	char str2[50];

	printf("ù��° ���ڿ�>>");
	scanf_s("%s", str1, 50);
	printf("�ι�° ���ڿ�>>");
	scanf_s("%s", str2, 50);

	if (strcmp(str1, str2) == 0)
	{
		printf("�����ϴ�\n");
	}
	else {
		printf("�ٸ��ϴ�\n");
	}

	return 0;
}