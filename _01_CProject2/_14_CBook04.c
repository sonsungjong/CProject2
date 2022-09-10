#include <stdio.h>
#include <string.h>

int main1404()
{
	int pos_num = 1;
	int num = 0;
	int i = 0;
	int count = 0;
	char num_string[4] = "123";

	// 문자열 길이 구하기 (strlen 은 null이 제외된 값을 반환함)
	count = strlen(num_string);				// 3 (null문자 제외됨)

	// 최고 자리수 구하기
	for (i = 0; i < count - 1; i++) {
		pos_num = pos_num * 10;
	}

	// 문자열을 숫자로 변환 - '0'
	for (i = 0; i < count; i++) {
		num = num + (num_string[i] - '0') * pos_num;
		pos_num = pos_num / 10;
	}

	printf(" %s -> %d \n", num_string, num);

	return 0;
}