#include <stdio.h>

int main0909() {

	for (int k = 2; k <= 9; k++) {
		printf("====%d´Ü====\t", k);
	}
	printf("\n");

	for (int i = 1; i <= 9; i++) {
		for (int j = 2; j <= 9; j++) {
			printf("%d * %d = %d\t", j, i, i * j);
		}
		printf("\n");
	}

	return 0;
}