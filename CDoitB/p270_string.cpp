#include <iostream>
#include <string>			// namespace (C++)
//#include <cstdio>			// stdio.h  (+namespace)

int main270() {
	//int data_length = 0;
	//std::string data = "happy";
	//data_length = strlen(data.c_str());
	//printf("data length = %d\n", data_length);

	//std::string data = "abc";
	//char result[16] = { 0 };
	//std::string result = "";

	//strcpy_s(result, 16, data.c_str());					// result 에 data 문자열을 복사
	//strcat_s(result, 16, "def");							// result 에 def 문자열을 붙이기
	//result = data;						// result 에 data 대입
	//result = result + "def";			// result 에 def 붙이기
	//
	//printf("%s + \"def\" = %s\n", data.c_str(), result.c_str());

	std::string data = "Hello";
	std::string result = "";
	result = data;
	result = result + " World";

	printf("%s\n", result.c_str());



	return 0;
}