#include <stdio.h>

int main0911() {
	int m;
	for (m = 5; m < 8; m++) {
		if (m == 6) continue;
		printf("m(%d)\n", m);
	}

	return 0;
}