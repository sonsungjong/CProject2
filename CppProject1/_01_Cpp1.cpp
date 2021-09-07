#include <cstdio>
// 자료형 확장: 변수크기정의 -> 함수까지

// 전역변수
int g_a;

class MyData {
// private: 외부에서 사용 불가능하게 함 (함수를 많이 만들어쓰게하려고 만듦)
private:
	// 매개변수
	int m_a;
	int m_b;

// public: 외부에서 사용 가능하게 함
public:
	void SetA(int a) {
		m_a = a;
	}

	void SetB(int b) {
		m_b = b;
	}

	// 구조체 안으로 함수를 이동
	int Sum()		// struct MyData* ap_data 인자 생략
	{
		// 지역변수
		int a;

		return m_a + m_b;
	}

	void ShowData()
	{
		printf("a : %d, b : %d\n", m_a, m_b);
	}
};

//int Sum(struct MyData* ap_data)
//{
//	return ap_data->a + ap_data->b;
//}

int main1day() {
	//struct MyData data = { 5, -2 };
	MyData data;
	int result;

	// private 멤버변수라도 함수로 사용가능
	data.SetA(5);
	data.SetB(-2);

	/*
	int* p = (int*)&data;		// 포인터문법
	*p = 7;				// m_a를 변경
	*(p + 1) = 8;		// m_b 를 변경
	*/

	result = data.Sum();		// data가 Sum함수를 사용		result = Sum(&data)

	printf("%d\n", result);

	return 0;
}

/*
	Cpp = C + 객체지향
	객체란 무엇인가?
	Object: 일상적인 행위나 작업을 표준화 시켜서 하나의 제품으로 만들어 놓은 것
	전체적인 공정 및 기능을 표준화시켜서 물건으로 만듦

	절차지향(C) : 바리스타가 커피를 만드는 과정
	객체지향(Cpp) : 커피제조기 만들기
*/