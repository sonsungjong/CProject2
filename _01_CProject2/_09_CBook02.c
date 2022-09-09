#include <stdio.h>

int main0902() {
	int sum = 0;
	int num = 1;
	for (num = 1;num<101;num++) {
		printf("num(%d) + sum(%d) = ", num, sum);
		sum += num;
		
	}
	printf("\nResult : num = %d sum = %d\n", num, sum);

	return 0;
}