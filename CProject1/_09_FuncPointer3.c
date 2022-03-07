#include <stdio.h>

// 함수의 원형이 유사한 
int Add(int a, int b) {
	return a + b;
}

int Sub(int a, int b) {
	return a - b;
}

int Mul(int a, int b) {
	return a * b;
}

int Div(int a, int b) {
	return b ? a / b : 0;
}

int main() {
	int (*pf_op_table[4])(int, int) = {Add, Sub, Mul, Div};

	char select = 'y';
	int a, b, op, result;
	while (select == 'y') {
		printf("연산한 두 개의 숫자를 입력해주세요(ex, 2 7) : ");
		scanf_s("%d %d", &a, &b);

		printf("제공되는 연산자 ( +:0, -:1, *:2, /:3\n");
		printf("어떤 연산을 수행할 것인지 입력하세요(ex, 2) : ");
		scanf_s("%d", &op);

		result = pf_op_table[op%4](a, b);
		printf("연산 결과 : %d\n", result);

		// 4가지만 제공으로 제약됨 (사용자는 늘릴 수 없고 개발자만 늘릴 수 있음)
		switch (op) {
		case 0:
			result = Add(a, b);
			printf("연산 결과 : %d\n", result);
			break;
		case 1:
			result = Sub(a, b);
			printf("연산 결과 : %d\n", result);
			break;
		case 2:
			result = Mul(a, b);
			printf("연산 결과 : %d\n", result);
			break;
		case 3:
			result = Div(a, b);
			printf("연산 결과 : %d\n", result);
			break;
		default:
			printf("제공되지 않는 연산자를 선택했습니다.\n");
		}
		printf("연산을 계속 하시겠습니까? (y or n) : ");
		rewind(stdin);				// %c 입력을 위해 stdin 초기화
		scanf_s("%c", &select, 1);
	}
	return 0;
}