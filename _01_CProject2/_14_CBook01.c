#include <stdio.h>

int main1401()
{
	char input_string[10];
	/* 반환값이 NULL이 아니라면 문자열을 입력받아서 input_string에 저장함 */
	if (NULL != gets(input_string)) {						// 컨트롤 + C를 하면 입력취소
		printf("input : %s \n", input_string);
	}
	else {
		printf("input -> Canceled \n");
	}

	return 0;
}