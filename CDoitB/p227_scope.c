#include <stdio.h>

int g_num;				// 전역변수 (어디서든지 사용 가능)
int Sum3(int num1, int num2);
extern int result;
//extern int staticnum;

int ReturnNum() 
{
	int num = 5;			// ReturnNum의 지역변수
	g_num = 1;
	
	return num;
}



int main227() 
{
	static int staticnum = 3;			// 지역에서 만드는 전역변수
	const int num2 = 3;				// const : 한번 정하면 못바꿈 (상수)

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