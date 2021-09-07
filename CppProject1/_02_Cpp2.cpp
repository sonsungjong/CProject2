#include <iostream>
#include <string>
#include <malloc.h>

using namespace std;

class MY_String {
private:
	int m_len;
	char* mp_str;

public:
	MY_String()		// 생성자
	{
		init();
	}
	~MY_String()		// 파괴자
	{
		free(mp_str);
	}

	void init()
	{
		m_len = 0;
		mp_str = NULL;
	}

	void SetString(const char* ap_str) {
		Destroy();
		m_len = strlen(ap_str) + 1;

		mp_str = (char*)malloc(m_len);
		if (mp_str != NULL) {
			memcpy(mp_str, ap_str, m_len);
		}
	}

	void Destroy() {
		free(mp_str);
		init();
	}

	void ShowString() {
		cout << mp_str << endl;
	}
};

int main2day2() {
	// 클래스 동적할당
	MY_String* p_str = new MY_String;
	p_str->SetString("tips");
	p_str->ShowString();
	delete p_str;

	int* c_p = (int*)malloc(sizeof(int) * 10);
	int* p_cpp = new int[10];
	int* p_one = new int;

	free(c_p);			// C형태의 메모리로케이션 해제할 때
	delete[] p_cpp;		// 배열new 해제할때
	delete p_one;			// new 해제할때

	return 0;
}