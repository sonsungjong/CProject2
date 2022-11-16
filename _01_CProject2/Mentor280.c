#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main_280()
{
	char* s1 = "Take a look!";
	char* s2 = (char*)malloc(sizeof(char) * strlen(s1) + 1);

	strcpy_s(s2,13,s1);

	printf("%s\n", s2);
	if (s2 != NULL) {
		free(s2);
	}



	return 0;
}