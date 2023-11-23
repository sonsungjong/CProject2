#include <stdio.h>
#include <iostream>

int main265() {
	char data[6] = { 'h','a','p','p','y','\0'};
	char ment[] = "C programming~";
	std::string str = "C programming~";

	printf("%s\n", data);
	printf("%s\n", ment);
	printf("%s\n", str.c_str());
	std::cout << str << std::endl;

	return 0;
}