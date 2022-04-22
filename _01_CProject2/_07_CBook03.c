#include <stdio.h>

void main0703() {
	// 증감연산자

	int x = 5;
	x = x + 2;
	printf("%d\n", x);

	int y = 5;
	y += 3;				// y = y+3;
	printf("%d\n", y);

	int i = 5;
	i++;					// i = i + 1; i += 1;
	printf("%d\n", i);

	int a = 0;
	a += 1;
	printf("%d\n", a);
	a -= 2;
	printf("%d\n", a);
	a *= 2;
	printf("%d\n", a);
	a /= 2;
	printf("%d\n", a);

	int b = 0;
	b++;
	printf("%d\n", b);
	++b;
	printf("%d\n", b);
	b--;
	printf("%d\n", b);
	--b;
	printf("%d\n", b);

}