#include <iostream>

using namespace std;

int g_a;

class CMyData
{
private:
	// 외부에서 해당 변수를 못쓰게함 (함수로 접근)
	int m_a;
	int m_b;

public:
	// 외부에서 사용할 수 있음
	int member;

	int Sum()		// CMyData *this
	{
		int a;
		int b;

		return m_a + m_b;
	}

	void SetA(int a)
	{
		this->m_a = a;
	}
	void SetB(int b)
	{
		m_b = b;
	}
};

int main_접근권한제어자()
{
	//CMyData data = { 5,-2 };
	CMyData data;		// private의 멤버변수는 다른 곳에서 접근할 수 없음
	int result;

	data.member = 1;
	//data.m_a = 7;
	//data.m_b = 8;
	data.SetA(7);		// 캡슐화: 객체의 내부자료를 외부에서 못쓰게함 (보안성?, 유지보수?) --> 함수생성을 유도
	data.SetB(8);		// 접근제한자를 통해서 함수생성을 유도함... 객체지향개념 연습용

	result = data.Sum();
	cout << result << endl;

	// private 포인터 접근
	int* p = (int*)&data;
	*p = 99;
	*(p + 1) = 100;
	result = data.Sum();
	cout << result << endl;

	return 0;
}

/*
	C의 구조체는 함수를 갖을 수 없고
	C++의 구조체는 함수를 포함할 수 있기 때문에
	C의 구조체와 C++의 구조체는 호환이 안되는 문제가 발생.
	그래서, 함수를 포함하는 구조체는 클래스라는 문법으로 이름을 바꿈
	struct --> class

	--접근제한자--
	struct의 기본값은 public
	class의 기본값은 private

	캡슐화: 객체의 내부자료를 외부에서 못쓰게함 (보안성?, 유지보수?) --> 함수생성 유도
	접근제한자를 통해서 함수생성을 유도함... 객체지향개념 스타일 연습용
	함수는 외부변화에 대처할 수 있기 때문에 public

	C : 함수가 데이터를 사용하는 방식, 변화에 취약함
	C++ : C에 객체지향이 추가, 인터페이스로 인해 처리속도가 느려짐

	C: 절차지향 - 바리스타
	C++: 객체지향 (표준화) - 자판기
*/