#include <iostream>
#include <string>
#include <malloc.h>

using namespace std;

int main_malloc() {
	const char* p_src_str = "sonsungjong";
	int len = strlen(p_src_str);		// 11 (NULL���� ���ܵ�)
	char* p_dest_str = (char*)malloc(len + 1);		// �޸� �Ҵ�
	
	strcpy_s(p_dest_str, len + 1, p_src_str);
	cout << p_dest_str << endl;

	free(p_dest_str);
	return 0;
}

/*
	�ٽ� : Ŭ������ ����
	���ٽ� : ������� Ŭ������ ���α׷���
*/