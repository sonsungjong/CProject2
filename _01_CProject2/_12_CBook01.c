#include <stdio.h>

/* ���ڿ��� ���̸� ���ϴ� �Լ� */
int MyStrlen(char data[])
{
	int count = 0;
	while (data[count] != 0)
	{
		count++;
	}
	return count;
}

int main1201()
{
	int data_length;
	char data[10] = { 'h','a','p','p','y' ,'\0',};
	data_length = MyStrlen(data);
	printf("data length = %d\n", data_length);

	return 0;
}