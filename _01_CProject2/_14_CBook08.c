#include <stdio.h>
#include <string.h>

int main1408()
{
	/* ���� 3�� */
	//int input_int1;
	//int input_int2;
	//int input_int3;
	//printf("�Է��ϼ��� >>");
	//scanf_s("%d %d %d", &input_int1, &input_int2, &input_int3);
	//printf("%d %d %d", input_int1, input_int2, input_int3);

	/* �ΰ� ���ڿ� strcat ��ġ�� */
	//char str1[16];
	//char str2[16];
	//memset(str1, 0, 16);
	//memset(str2, 0, 16);

	//scanf_s("%s", str1, 16);
	//scanf_s("%s", str2, 16);

	//strcat(str1, str2);
	//printf("%s", str1);


	/* �Է�&��� ���α׷� */
	//char name[16];
	//float weight;
	//float height;
	//char blood_type;
	//int heart;

	//int scan_result = scanf("%s %f %f %c %d", name, &weight, &height, &blood_type, &heart);
	//if (scan_result == 0) {
	//	printf("�߸� �Է��ϼ̽��ϴ�. \n");
	//}
	//else {
	//	printf("�̸�: %s, ������ : %.1fkg, Ű: %.1fcm, ������ : %c��, �ɹڼ� : %dȸ", name, weight, height, blood_type, heart);
	//}

	/*���� ó�� ���α׷� �����*/
	char name[3][16];
	int score[3][6];

	for (int i = 0; i < 3; i++) {
	printf("%d ��° �л� �̸�: ", i+1);
	(void)scanf("%s", name[i]);
	printf("���� ����: ");
	(void)scanf("%d", &(score[i][0]));
	printf("���� ����: ");
	(void)scanf("%d", &(score[i][1]));
	printf("���� ����: ");
	(void)scanf("%d", &(score[i][2]));
	printf("\n");

	score[i][3] = score[i][0] + score[i][1] + score[i][2];				// ����
	score[i][4] = (score[i][3]) / 3;				// ���
	}

	if (score[0][4] > score[1][4]) {
		if (score[1][4] > score[2][4]) {
			score[0][5] = 1;
			score[1][5] = 2;
			score[2][5] = 3;
		}
		else {
			if (score[0][4] > score[2][4]) {
				score[0][5] = 1;
				score[1][5] = 3;
				score[2][5] = 2;
			}
			else {
				score[0][5] = 2;
				score[1][5] = 3;
				score[2][5] = 1;
			}
		}
	}
	else {			// 123, 132, 213, 231, 321, 312
		if (score[2][4] > score[1][4]) {
			score[0][5] = 3;
			score[1][5] = 2;
			score[2][5] = 1;
		}
		else {
			if (score[0][4] > score[2][4]) {
				score[0][5] = 2;
				score[1][5] = 1;
				score[2][5] = 3;
			}
			else {
				score[0][5] = 3;
				score[1][5] = 1;
				score[2][5] = 2;
			}
		}
	}

	printf("\n------------------------------------------------------------------------\n");
	printf("�̸�\t\t����\t����\t����\t����\t���\t���");
	printf("\n------------------------------------------------------------------------\n");
	if (score[0][5] < score[1][5] && score[0][5] < score[2][5]) {				// 1��°�� 1��
		printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[0], score[0][0], score[0][1], score[0][2], score[0][3], score[0][4], score[0][5]);
		if (score[1][5] < score[2][5]) {
			// 2��°�� 2��
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[1], score[1][0], score[1][1], score[1][2], score[1][3], score[1][4], score[1][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[2], score[2][0], score[2][1], score[2][2], score[2][3], score[2][4], score[2][5]);
		}
		else {
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[2], score[2][0], score[2][1], score[2][2], score[2][3], score[2][4], score[2][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[1], score[1][0], score[1][1], score[1][2], score[1][3], score[1][4], score[1][5]);
		}
	}
	else if (score[1][5] < score[0][5] && score[1][5] < score[2][5]) {				// 2��°�� 1��
		printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[1], score[1][0], score[1][1], score[1][2], score[1][3], score[1][4], score[1][5]);
		if (score[0][5] < score[2][5]) {
			// 1��°�� 2��
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[0], score[0][0], score[0][1], score[0][2], score[0][3], score[0][4], score[0][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[2], score[2][0], score[2][1], score[2][2], score[2][3], score[2][4], score[2][5]);
		}
		else {
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[2], score[2][0], score[2][1], score[2][2], score[2][3], score[2][4], score[2][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[0], score[0][0], score[0][1], score[0][2], score[0][3], score[0][4], score[0][5]);
		}
	}
	else {				// 3��°�� 1��
		printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[2], score[2][0], score[2][1], score[2][2], score[2][3], score[2][4], score[2][5]);
		if (score[0][5] < score[1][5]) {
			// 1��°�� 2��
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[0], score[0][0], score[0][1], score[0][2], score[0][3], score[0][4], score[0][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[1], score[1][0], score[1][1], score[1][2], score[1][3], score[1][4], score[1][5]);
		}
		else {
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[1], score[1][0], score[1][1], score[1][2], score[1][3], score[1][4], score[1][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d��\n", name[0], score[0][0], score[0][1], score[0][2], score[0][3], score[0][4], score[0][5]);
		}
	}
	

	return 0;
}