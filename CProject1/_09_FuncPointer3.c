#include <stdio.h>

// �Լ��� ������ ������ 
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
		printf("������ �� ���� ���ڸ� �Է����ּ���(ex, 2 7) : ");
		scanf_s("%d %d", &a, &b);

		printf("�����Ǵ� ������ ( +:0, -:1, *:2, /:3\n");
		printf("� ������ ������ ������ �Է��ϼ���(ex, 2) : ");
		scanf_s("%d", &op);

		result = pf_op_table[op%4](a, b);
		printf("���� ��� : %d\n", result);

		// 4������ �������� ����� (����ڴ� �ø� �� ���� �����ڸ� �ø� �� ����)
		switch (op) {
		case 0:
			result = Add(a, b);
			printf("���� ��� : %d\n", result);
			break;
		case 1:
			result = Sub(a, b);
			printf("���� ��� : %d\n", result);
			break;
		case 2:
			result = Mul(a, b);
			printf("���� ��� : %d\n", result);
			break;
		case 3:
			result = Div(a, b);
			printf("���� ��� : %d\n", result);
			break;
		default:
			printf("�������� �ʴ� �����ڸ� �����߽��ϴ�.\n");
		}
		printf("������ ��� �Ͻðڽ��ϱ�? (y or n) : ");
		rewind(stdin);				// %c �Է��� ���� stdin �ʱ�ȭ
		scanf_s("%c", &select, 1);
	}
	return 0;
}