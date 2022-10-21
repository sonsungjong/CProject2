#include <stdio.h>
#include <string.h>				// ���ڿ� ó�� �Լ� ����

int main_205()
{
	/*
		strlen : ���ڿ� ���� ��ȯ (null����)
		strcpy : ù��°�� �ι�° ���ڿ��� ����
		strncpy : ù��°�� �ι�° ���ڿ��� ���ϴ� ������ŭ�� ����
		strcat : ù��°�� �ι�° ���ڿ��� �̾���̱�
		strncat : ù��°�� �ι�° ���ڿ��� ���ϴ� ������ŭ�� �̾���̱�
		strcmp : ���� ������ 0, �ٸ��� 0 �̿��� ����
		strncmp : ���ϴ� ���������� ���� ���ϰ� ������ 0, �ٸ��� �׿� ����
	*/

	char str1[50] = "apple is good";
	char str2[50] = "berry is good";
	char str3[50];

	// ���ڿ� ����
	int len_str1 = strlen(str1);
	int len_str2 = strlen(str2);
	printf("str1�� ���� : %d, str2�� ���� : %d \n", len_str1, len_str2);

	// str3�� str1�� ����
	strcpy(str3, str1);

	// str2�� str1�� ������ 5ĭ�� ����
	strncpy(str2, str1, 5);

	printf("%s\n %s\n %s\n", str1, str2, str3);

	char str4[50] = "Michael ";
	char str5[50] = "Michael ";

	// str4�� ���ڿ� ���̱�
	strcat(str4, "Bolton");
	printf("%s\n", str4);

	// str5�� 7���� ���ڿ� ���̱�
	strncat(str5, "Jackson Five", 7);
	printf("%s\n", str5);

	// str4�� str5 ���ϱ� (��ġ�ϸ� 0, �ٸ��� 1)
	int result = strcmp(str4, str5);
	printf("�� ��� : %d\n", result);

	result = strncmp(str4, str5, 7);
	printf("�� ��� : %d\n", result);

	return 0;
}