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

	printf("������ %d\n", total);

	// #2
	char str[50];
	char u_str[50] = { 0, };
	printf("���ڿ��� �Է��ϼ��� >>");
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
	printf("���ܾ� �Է� >>");
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

	printf("�ƽ�Ű�ڵ尪�� ���� ū ���� : %c \n", ascii);

	return 0;
}