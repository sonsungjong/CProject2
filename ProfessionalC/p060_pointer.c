#include <stdio.h>

/*
	포인터 변수의 산술연산
	메모리는 아주 긴 1차원의 바이트 배열이다.
	포인터를 증가시키면 앞으로 가고, 감소시키면 뒤로 간다
	포인터를 1 증가시키면 메모리에서 1바이트 이상 앞으로 나아간다
	포인터를 1 증감시킬 때 움직이는 바이트 숫자를 나타내기 위해 포인터의 C 데이터 타입을 결정한다
	이러한 포인터의 산술연산 간격은 C 자료형에 의해 결정된다.

	예를들면, int* 와 char* 는 크기가 같지만 산술연산 간격이 다르다.
	int*는 4바이트의 연산 간격을 갖고, char*는 1바이트의 연산 간격을 갖는다
*/
int main060(int argc, char** argv)
{
	int var = 1;

	int* int_ptr = 0;				// 포인터를 NULL로 만들기
	int_ptr = &var;

	char* char_ptr = 0;
	char_ptr = (char*)&var;

	printf("Before arithmetic: int_ptr: %u, char_ptr: %u\n", (unsigned int)int_ptr, (unsigned int)char_ptr);

	int_ptr++;				// 산술연산 간격 4바이트
	char_ptr++;			// 산술연산 간격 1바이트

	// 정수 포인터의 연산 간격은 4바이트, 문자 포인터의 연산 간격은 1바이트
	printf("After arithmetic: int_ptr: %u, char_ptr: %u\n", (unsigned int)int_ptr, (unsigned int)char_ptr);


	int arr[5] = { 9, 22, 30, 23, 18 };

	// 정수 배열의 모든 원소 출력 (포인터 미사용)
	for (int i = 0; i < 5; i++) {
		printf("%d\n", arr[i]);
	}

	// 정수 배열의 모든 원소 출력 (포인터)
	int* ptr = &arr[0];
	for (;;) {
		printf("%d\n", *ptr);
		if (ptr == &arr[5 - 1]) {
			break;
		}
		ptr++;
	}

	return 0;
}