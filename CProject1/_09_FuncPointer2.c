#include <stdio.h>

typedef (*PFN_PV)(int a_value, char(*ap_data)[3]);

void PrintValue1(int a_value, char (*ap_data)[3]) {
	printf("Value = %d\n", a_value);
}

int main92() {
	void (*p_func)(int, char (*)[3]) = &PrintValue1;						// &생략가능
	PFN_PV p_func1 = PrintValue1;
	
	(*p_func)(5, NULL);

	p_func1(6, NULL);

	return 0;
}