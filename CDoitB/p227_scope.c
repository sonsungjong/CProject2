#include <stdio.h>

int g_num;				// �������� (��𼭵��� ��� ����)
int Sum3(int num1, int num2);
extern int result;
//extern int staticnum;

int ReturnNum() 
{
	int num = 5;			// ReturnNum�� ��������
	g_num = 1;
	
	return num;
}



int main227() 
{
	static int staticnum = 3;			// �������� ����� ��������
	const int num2 = 3;				// const : �ѹ� ���ϸ� ���ٲ� (���)

	int num = 0;
	num = ReturnNum();
	//g_num = 3;
	//printf("%d\n", num);

	//printf("%d\n", g_num);

	Sum3(3, 4);
	printf("%d\n", result);

	
	printf("%d", staticnum);

	return 0;
}