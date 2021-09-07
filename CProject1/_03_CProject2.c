#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define POW(x) (x*x)

// pointer : 일반변수를 사용하게 됨으로써, 명령어 레벨단위의 제어권을 뺐꼈던 것을 다시 사용 (간접주소명령)
int main3회() {
	// define
	int in = POW(3);

	char *c= "1.55";
	char ch[16];
	float floa = 1.0f;
	int num1;

	floa = atof(c);		// 문자열 -> 실수
	printf("%.2f\n", floa);

	sprintf(ch, "%f", floa);		// 실수 -> 문자열
	printf("%.4s\n", ch);

	num1 = ch[0] - '0';	// 문자열 -> 정수 (아스키코드 이용)
	printf("%d\n", num1);

	// 명령어 기술 -> 자료형 기술로 바꿈 (잘못된명령을 방지하기위해 바꿨으나 이해가 안가게됨)
	// 명사 == 변수 (철수가, 영희가)
	// 대명사 == 포인터 (그가, 그녀가)

	// basepoint : 상대주소


	return 0;
}