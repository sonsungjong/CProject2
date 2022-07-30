#include <stdio.h>

int main0908() {
	for (int step = 2; step <= 9; step++) {
		for (int i = 1; i <= 9; i++) {
			printf("%d * %d = %d\n", step, i, step * i);
		}
	}

	return 0;
}