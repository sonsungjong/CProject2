#include "pch.h"

void main()
{
	char str1[50];
	char str2[50];
	printf("문자열1 입력>>");
	scanf("%[^\n]s", str1);
	rewind(stdin);
	printf("문자열2 입력>>");
	scanf("%[^\n]s", str2);

	// 문자열이 같은지 비교
	if (strcmp(str1, str2) == 0) {
		printf("같습니다.");
	}
	else {
		printf("다릅니다.");
	}
	
	// 더 긴 문자열 검사
	if (strlen(str1) > strlen(str2)) {
		printf("%s\n", str1);
		printf("%s\n", str2);
	}
	else {
		printf("%s\n", str2);
		printf("%s\n", str1);
	}

	// 문자열 중 숫자만 추출하여 더하기
	printf("문자열3 입력>>");
	char str3[50] = { 0, };
	scanf("%s", str3);
	int sum = 0;
	for (int i = 0; i < 50; i++) {
		if (str3[i] > 47 && str3[i] < 58) {
			sum += str3[i] - 48;
		}
	}
	printf("%d\n", sum);
}
