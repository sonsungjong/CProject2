#include <iostream>
#include <stdlib.h>
// 연산자 오버로딩
using namespace std;
// 함수명으로 기능을 만들고 외워서 사용하게하니깐 번거로워서 연산자기호를 사용
// std.setString("gg"); --> str = "gg";
class Overloadd {
private:
	int m_len;
	char* mp_str;

public:
	// operator라는 키워드를 사용하면 뒤에오는 연산자를 함수명으로 인식
	void operator=(const char* ap_str)
	{
		m_len = strlen(ap_str) + 1;

		mp_str = (char*)malloc(m_len);
		if (mp_str != NULL) {
			memcpy(mp_str, ap_str, m_len);
		}
	}

	int ToInteger() {
		// stdlib.h 에 있음
		return atoi(mp_str);
	}

	double ToDouble() {
		return atof(mp_str);
	}

	// 형변환 연산자 오버로딩
	operator int()
	{
		// 컴파일러 자동형변환 처리
		return atoi(mp_str);
	}

	operator double()
	{
		// 연산자 오버로딩으로 자동형변환
		return atof(mp_str);
	}
};

int a;

int main연산자오버로딩()
{
	// 연산자 오버로딩 : 함수명을 연산자로 사용
	Overloadd str;
	int a;
	
	//str.operator=("tips");	이렇게 변환해줌
	str = "tipsoft";		// 함수명을 편하게 연산자로 만듦

	str = "100";
	int num;
	num = str.ToInteger();		// 함수정의
	num = str;							// 형변환 연산자오버로딩
	
	cout << "str" << endl;
	cout << num << endl;

	::a = 5;		// 전역변수에 대입
	a = 5;		// 지역변수에 대입
	// :: 스코프연산자 ~속에있는
	

	return 0;
}