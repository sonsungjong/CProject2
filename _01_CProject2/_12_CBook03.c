#include <stdio.h>
#include <string.h>

int main1203()
{
	char data[10] = { 'a','b','c', 0, };
	char result[16];
	int len;

	strcpy(result, data);						// strcpy : 앞 문자열에 뒷 문자열을 카피
	strcat(result, "def");						// strcat : 앞 문자열에 뒷 문자열을 이어붙이기
	len = strlen(result);						// strlen : 문자열의 길이를 null문자 제외하고 반환

	printf("%s + \"def\" = %s (%d)\n", data, result, len);

	return 0;
}