#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main_154()
{
	char greet[] = "Hello, Guys!";
	printf("1���� �迭 greet�� ���̴� : %lld\n", sizeof(greet));

	char mind[30] = "I Love programming!";
	printf("%s\n", mind);

	mind[7] = '\0';						// ���ڿ��� null���ڸ� �������� ���Ḧ �ν���
	printf("%s \n", mind);

	mind[1] = '\0';
	printf("%s \n", mind);

	char str[50];
	printf("���ڿ��� �Է��ϼ��� >>");
	(void)scanf("%s", str);				// ���ڿ��� & ������ �ʿ����
	printf("�Էµ� ���ڿ� : %s\n", str);

	return 0;
}