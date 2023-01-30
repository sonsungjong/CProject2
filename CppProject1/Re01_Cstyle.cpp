#include <cstdio>
// 중복코드 줄이기
// 근접한 위치의 중복코드는 '반복문'을 통해 줄인다
// 반복문은 떨어져있는 중복코드에 대해서는 대처가 불가능하다
// 떨어져있는 중복코드는 '함수'를 통해 줄인다
// 함수는 코드변화에 대처가 가능하다
// 매개변수의 갯수 또는 자료형이 바뀌면 함수 뿐만아니라 함수호출부까지 변경되어야하는 단점이 있다
// 매개변수에 '구조체'를 사용하면 해결할 수 있다
// 구조체 변수를 매개변수로 전달할 때마다 12바이트의 메모리복사가 발생하고 원본데이터 수정이 불가능하다
// 메모리 절약과 원본데이터 수정을 위해선 매개변수를 '구조체 포인터' 로 사용한다.
// *C언어 스타일 : 매개변수를 구조체 포인터로 취하는 함수

struct ValueList
{
	int a, b, c;
};

void ModifyValue(int* p_value, const char* p_var_name)
{
	if (*p_value < 0)
		printf("%s변수의 값을 %d에서 %d로 변경함\n", p_var_name, -*p_value, *p_value = -*p_value);
}

void MultiplyValue(struct ValueList* values)
{
	ModifyValue(&values->a, "a");
	ModifyValue(&values->b, "b");
	ModifyValue(&values->c, "c");

	printf("%d * %d * %d = %d\n", values->a, values->b, values->c, values->a * values->b * values->c);
}

int main()
{
	// 구조체 선언
	struct ValueList values;
	printf("곱셈할 수를 입력하세요>>");
	scanf_s("%d %d %d", &values.a, &values.b, &values.c);

	// 구조체의 주소를 전달
	MultiplyValue(&values);

	return 0;
}
