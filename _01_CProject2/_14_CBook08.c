#include <stdio.h>
#include <string.h>

int main1408()
{
	/* 정수 3개 */
	//int input_int1;
	//int input_int2;
	//int input_int3;
	//printf("입력하세요 >>");
	//scanf_s("%d %d %d", &input_int1, &input_int2, &input_int3);
	//printf("%d %d %d", input_int1, input_int2, input_int3);

	/* 두개 문자열 strcat 합치기 */
	//char str1[16];
	//char str2[16];
	//memset(str1, 0, 16);
	//memset(str2, 0, 16);

	//scanf_s("%s", str1, 16);
	//scanf_s("%s", str2, 16);

	//strcat(str1, str2);
	//printf("%s", str1);


	/* 입력&출력 프로그램 */
	//char name[16];
	//float weight;
	//float height;
	//char blood_type;
	//int heart;

	//int scan_result = scanf("%s %f %f %c %d", name, &weight, &height, &blood_type, &heart);
	//if (scan_result == 0) {
	//	printf("잘못 입력하셨습니다. \n");
	//}
	//else {
	//	printf("이름: %s, 몸무게 : %.1fkg, 키: %.1fcm, 혈액형 : %c형, 심박수 : %d회", name, weight, height, blood_type, heart);
	//}

	/*성적 처리 프로그램 만들기*/
	char name[3][16];
	int score[3][6];

	for (int i = 0; i < 3; i++) {
	printf("%d 번째 학생 이름: ", i+1);
	(void)scanf("%s", name[i]);
	printf("국어 점수: ");
	(void)scanf("%d", &(score[i][0]));
	printf("영어 점수: ");
	(void)scanf("%d", &(score[i][1]));
	printf("수학 점수: ");
	(void)scanf("%d", &(score[i][2]));
	printf("\n");

	score[i][3] = score[i][0] + score[i][1] + score[i][2];				// 총점
	score[i][4] = (score[i][3]) / 3;				// 평균
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
	printf("이름\t\t국어\t영어\t수학\t총점\t평균\t등수");
	printf("\n------------------------------------------------------------------------\n");
	if (score[0][5] < score[1][5] && score[0][5] < score[2][5]) {				// 1번째가 1등
		printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[0], score[0][0], score[0][1], score[0][2], score[0][3], score[0][4], score[0][5]);
		if (score[1][5] < score[2][5]) {
			// 2번째가 2등
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[1], score[1][0], score[1][1], score[1][2], score[1][3], score[1][4], score[1][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[2], score[2][0], score[2][1], score[2][2], score[2][3], score[2][4], score[2][5]);
		}
		else {
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[2], score[2][0], score[2][1], score[2][2], score[2][3], score[2][4], score[2][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[1], score[1][0], score[1][1], score[1][2], score[1][3], score[1][4], score[1][5]);
		}
	}
	else if (score[1][5] < score[0][5] && score[1][5] < score[2][5]) {				// 2번째가 1등
		printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[1], score[1][0], score[1][1], score[1][2], score[1][3], score[1][4], score[1][5]);
		if (score[0][5] < score[2][5]) {
			// 1번째가 2등
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[0], score[0][0], score[0][1], score[0][2], score[0][3], score[0][4], score[0][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[2], score[2][0], score[2][1], score[2][2], score[2][3], score[2][4], score[2][5]);
		}
		else {
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[2], score[2][0], score[2][1], score[2][2], score[2][3], score[2][4], score[2][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[0], score[0][0], score[0][1], score[0][2], score[0][3], score[0][4], score[0][5]);
		}
	}
	else {				// 3번째가 1등
		printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[2], score[2][0], score[2][1], score[2][2], score[2][3], score[2][4], score[2][5]);
		if (score[0][5] < score[1][5]) {
			// 1번째가 2등
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[0], score[0][0], score[0][1], score[0][2], score[0][3], score[0][4], score[0][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[1], score[1][0], score[1][1], score[1][2], score[1][3], score[1][4], score[1][5]);
		}
		else {
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[1], score[1][0], score[1][1], score[1][2], score[1][3], score[1][4], score[1][5]);
			printf("%s\t\t%d\t%d\t%d\t%d\t%d\t%d등\n", name[0], score[0][0], score[0][1], score[0][2], score[0][3], score[0][4], score[0][5]);
		}
	}
	

	return 0;
}