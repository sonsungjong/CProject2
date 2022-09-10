#include <stdio.h>

int main1403()
{
	int pos_num = 100;
	int num = 0;
	int temp_num = 0;
	int i = 0;

	char num_string[4] = "123";
	for (i = 0; i < 3; i++) {
		temp_num = num_string[i] - '0';
		num = num + temp_num * pos_num;
		pos_num = pos_num / 10;
	}
	printf(" %s -> %d \n", num_string, num);
	return 0;
}