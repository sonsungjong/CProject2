#include <iostream>
#include <string>
#include <malloc.h>

using namespace std;

int main_malloc() {
	const char* p_src_str = "sonsungjong";
	int len = strlen(p_src_str);		// 11 (NULL문자 제외됨)
	char* p_dest_str = (char*)malloc(len + 1);		// 메모리 할당
	
	strcpy_s(p_dest_str, len + 1, p_src_str);
	cout << p_dest_str << endl;

	free(p_dest_str);
	return 0;
}

/*
	핵심 : 클래스를 만듦
	비핵심 : 만들어진 클래스로 프로그래밍
*/