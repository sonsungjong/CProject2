#include <stdio.h>

/* 입력 문자갯수를 초과했을 때 필터링해주는 입력받기 */
int GetMyString(char buffer[], int limit)
{
	int i;
	for (i = 0; i < limit; i++) {
		buffer[i] = getchar();
		if (buffer[i] == '\n') {
			buffer[i] = 0;
			return 1;
		}
	}
	buffer[i] = 0;
	rewind(stdin);					// 표준 입력 버퍼에 남아있는 문자들을 제거함 (입력버퍼 초기화)
	return 0;
}

int main1402()
{
	char temp[10];
	int state;
	state = GetMyString(temp, 9);

	if (state == 1) {
		printf("input : %s\n", temp);
	}
	else {
		printf("input : %s -> out of range \n", temp);
	}

	return 0;
}