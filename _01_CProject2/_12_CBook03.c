#include <stdio.h>
#include <string.h>

int main1203()
{
	char data[10] = { 'a','b','c', 0, };
	char result[16];
	int len;

	strcpy(result, data);						// strcpy : �� ���ڿ��� �� ���ڿ��� ī��
	strcat(result, "def");						// strcat : �� ���ڿ��� �� ���ڿ��� �̾���̱�
	len = strlen(result);						// strlen : ���ڿ��� ���̸� null���� �����ϰ� ��ȯ

	printf("%s + \"def\" = %s (%d)\n", data, result, len);

	return 0;
}