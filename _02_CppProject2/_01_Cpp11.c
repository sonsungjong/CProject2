#include <stdio.h>
// this 포인터에 대해서

// 구조체
struct MyData
{
	int a;
	int b;
};

// 함수
int Sum(struct MyData* ap_data)
{
	return ap_data->a + ap_data->b;
}

int main11()
{
	struct MyData data = { 5, -2 };
	int result;

	result = Sum(&data);
	printf("%d\n", result);

	return 0;
}

/*
	함수 : 코드의 변화를 대처
	구조체 : 자료형의 변화를 대처
	포인터 : 메모리의 사용을 줄이고자 복사빈도를 줄임

	자료형 == 법

	1. 클래스, this포인터, 접근권한제어자
	2. 생성자, 파괴자, new, delete
	3. 오버로딩, namespace, 스코프
	4. 상속
*/