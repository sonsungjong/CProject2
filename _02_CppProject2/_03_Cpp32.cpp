#include <iostream>
// 연산자 오버로딩

using namespace std;

void showString();

class Cpp32 
{
private:
	char* mp_str;
	int m_len;
	int m_max_len;
public:
	Cpp32(){
		init();
	}
	~Cpp32(){
		destroy();
	}
	void setString(const char* ap_str) {
		m_len = strlen(ap_str) + 1;
		if (m_max_len < m_len)
		{
			free(mp_str);
			m_max_len = m_len;
			mp_str = (char*)malloc(m_max_len);
		}
		memcpy(mp_str, ap_str, m_len);
	}

	
	void init() {
		m_len = 0;
		mp_str = NULL;
	}
	void destroy()
	{
		free(mp_str);
		init();
	}
	void showString();
	int toInt()
	{
		return atoi(mp_str);		// "100" -> 100
	}
	// 형변환연산자 오버로딩은 자료형X
	operator int() {
		return atoi(mp_str);
	}		//  명시하지 않아도 알아서 컴파일러가 사용함
	double toDouble()
	{
		return atof(mp_str);		// "100.5" -> 100.5
	}
	operator double() {
		::showString();		// 파일 전역에 있는 showString() 함수를 사용
		return atof(mp_str);
	}
	// 대입연산자 오버로딩
	void operator=(const char* ap_str)
	{
		m_len = strlen(ap_str) + 1;
		if (m_max_len < m_len)
		{
			free(mp_str);
			m_max_len = m_len;
			mp_str = (char*)malloc(m_max_len);
		}
		memcpy(mp_str, ap_str, m_len);
	}
};

int a;
void showString() {
	cout << "자동 형변환" << endl;
}

// 스코프로 다른 클래스안의 함수 접근, 분리하면 소스파일 보여줄 필요없음 (헤더와 소스 분리)
void Cpp32::showString()
{
	cout << mp_str << endl;
}

int main32()
{
	int a;
	Cpp32 str;
	// 대입연산자 오버로딩
	str = "100000";
	str.showString();
	str = "안녕하세요!";
	str.showString();
	str.setString("안녕하세요");
	str.showString();
	str = "30";
	str.showString();

	//int num = str.toInt();
	int num1 = str;		// int형변환연산자 오버로딩이 자동으로 사용됨
	double num2 = str;		// double형변환연산자 오버로딩이 자동으로 사용

	::a = 5;		// 전역변수
	a = 5;		// 지역변수

	return 0;
}

/*
	연산자 오버로딩 : 함수이름을 연산자로 사용, 보기에 좀더 직관적
	str.setString("") --> str = ""
	이름짓기의 어려움, 이름을 외워야하는 번거로움 해소
	사용자 입장에서 편하게 사용
*/