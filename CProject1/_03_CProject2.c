#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define POW(x) (x*x)

// pointer : �Ϲݺ����� ����ϰ� �����ν�, ��ɾ� ���������� ������� ������ ���� �ٽ� ��� (�����ּҸ��)
int main3ȸ() {
	// define
	int in = POW(3);

	char *c= "1.55";
	char ch[16];
	float floa = 1.0f;
	int num1;

	floa = atof(c);		// ���ڿ� -> �Ǽ�
	printf("%.2f\n", floa);

	sprintf(ch, "%f", floa);		// �Ǽ� -> ���ڿ�
	printf("%.4s\n", ch);

	num1 = ch[0] - '0';	// ���ڿ� -> ���� (�ƽ�Ű�ڵ� �̿�)
	printf("%d\n", num1);

	// ��ɾ� ��� -> �ڷ��� ����� �ٲ� (�߸��ȸ���� �����ϱ����� �ٲ����� ���ذ� �Ȱ��Ե�)
	// ��� == ���� (ö����, ����)
	// ���� == ������ (�װ�, �׳డ)

	// basepoint : ����ּ�


	return 0;
}