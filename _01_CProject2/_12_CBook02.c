#include <stdio.h>
#include <string.h>

int main1202()
{
	int data_length;
	char data[10] = { 'h','a','p','p','y', '\0' };
	data_length = strlen(data);							// null문자를 제외한 문자열 길이 반환
	printf("data length = %d\n", data_length);

	return 0;
}