#include <stdio.h>

void main1407()
{
	int num = 0;
	while (1) {
		printf("input age >> ");
		int scan_result = scanf("%d", &num);
		if (scan_result == 0) {
			rewind(stdin);					// �Է� ���� ��� ����
			printf("[Enter] digit number!! \n");
		}
		else {
			if (num > 0 && num <= 130) {
				break;
			}
			else {
				printf("Incorrect Age!! \n");
			}
		}
	}
	printf("your age : %d \n", num);
}