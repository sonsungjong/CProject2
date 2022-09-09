#include <stdio.h>

// extern : 다른 파일의 전역변수를 참조하겠다 (같은 이름의 이중 전역변수 선언 방지)

void ExSum(int, int);
extern int result;							// 같은 프로젝트 다른파일의 전역변수를 참조해옴 extern (전역변수 두개존재 링크오류 방지)

//int g_data1;										// 같은 프로젝트 다른 파일에 이미 g_data1 이 있으므로 링크오류가 발생할 수 있음
extern int g_data2;
//extern int g_data3;								// 선언되지 않은 전역변수를 참조하는 것은 위험
extern int g_data4 = 0;							// 단, 초기화까지 같이 해주면 선언되지 않은 전역변수라도 새로 생성함
// extern int g_data5 = 0;					// 이미 초기화하여 선언된 다른 파일의 전역변수를 extern참고하여 초기화하려고 하고있음.

int main1101()
{
	ExSum(5, 3);
	printf("5+3 = %d\n", result);

	return 0;
}