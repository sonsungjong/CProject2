#include <stdio.h>

// PrintValue 함수는 라이브러리 제공
// 함수포인터의 아규먼트를 만들어서 사용자가 수정이 가능하게 만듦
void PrintVal(int a_value, void (*ap_modify)(int*))
{
	if (ap_modify != NULL) { (*ap_modify)(&a_value); }
	printf("Value = %d\n", a_value);
}

// Modify Value 함수는 라이브러리 사용자가 필요에 의해서 만듦
void ModifyAbsValue(int* ap_value)
{
	// ap_value 포인터가 가리키는 대상이 음수면 양수로 변경한다
	if (*ap_value < 0) { *ap_value = *ap_value * (-1); }
}

void ModifyZeroValue(int* ap_value)
{
	// ap_value 포인터가 기리키는 대상이 음수면 0으로 변경한다.
	if (*ap_value < 0) { *ap_value = 0; }
}

int main94() {
	int value = -5;

	PrintVal(value, NULL);			// -5가 출력
	
	// PrintValue 함수에서 함수포인터에 의해 ModifyAbsValue 함수가 호출되어 -5가 5로 변경됨.
	PrintVal(value, ModifyAbsValue);

	// PrintValue 함수에서 함수포인터에 의해 ModifyZeroValue 함수가 호출되어 -5가 0으로 변경
	PrintVal(value, ModifyZeroValue);

	return 0;
}
