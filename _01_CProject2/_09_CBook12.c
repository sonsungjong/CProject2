#include <stdio.h>

int main0912() {
	int m, n;
	for (m = 5; m < 7; m++) {
		for (n = 0; n < 3; n++) {
			if (m == 5 && n == 1) {
				goto exit_pos;						// exit_pos: ·Î º¸³¿
			}
			printf("m(%d)-n(%d)\n", m, n);
		}
	}
	exit_pos :
	printf("< end >\n", m, n);

	return 0;
}