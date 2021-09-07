#include <iostream>
// this 포인터에 대해서

using namespace std;

// 구조체
struct MyData
{
	int a;
	int b;

	// 함수
	//int Sum(struct MyData* ap_data)		// struct MyData* this (자신을 가리키는 포인터)를 생략하고 this로 포인터 변수를 제공
	int Sum()
	{
		int a;
		//return ap_data->a + ap_data->b;
		//return this->a + this->b;
		return this->a + b;
	}

	//void ShowData(struct MyData* ap_data)		// struct MyData* this (자신을 가리키는 포인터)를 생략하고 this로 포인터변수 제공
	void ShowData()
	{
		//cout << "a : " << ap_data->a << "b : " << ap_data->b << endl;
		//cout << "a : " << this->a << "b : " << this->b << endl;
		cout << "a : " << a << "b : " << b << endl;
	}
};

int main_this포인터()
{
	struct MyData data = { 5, -2 };
	int result;

	data.a = 7;
	data.b = 8;

	//result = Sum(&data);	구조체 안으로 들어갔으므로 객체명.함수() 로 사용
	//result = data.Sum(&data);		// 이 경우 data표현이 중복됨
	result = data.Sum();		// &data 를 생략하고 자기자신의 주소를 표현하는 this포인터를 제공

	cout << result << endl;

	return 0;
}

/*
	함수 : 코드의 변화를 대처
	구조체 : 자료형의 변화를 대처
	포인터 : 메모리의 사용을 줄이고자 복사빈도를 줄임

	자료형 == 법
	Cpp에서는 함수가 구조체를 가져다 쓰기 때문에 해당 구조체안에 함수를 넣어버림 -> 구조체는 여러 함수에서 사용할 수 있기 때문에 함수를 안에 넣음
	구조체 안에 함수는 컴파일 중에 분리됨 (구조체의 데이터크기는 변수크기의 합으로 계산)

	구조체 안으로 넣었기 때문에 표현이 중복되어서 생략하고 자기자신을 가리키는 문법을 this 포인터라고 만듦
	C : 실력자들이 운영체제를 만들기 위해 만들어진 언어
	C++ : 운영체제 안에서 응용 프로그램을 만들기 위해 만들어진 언어

	구조체 내부의 함수에서 그 함수를 감싸는 구조체의 주소를 가리키기 위한 표현을 생략하고 this라는 포인터를 제공
	this->

	하지만, 갑자기 this-> 라는 자기참조포인터가 나오기 때문에
	this-> 마저도 생략해버림

	대신, 내부에 같은 이름의 변수명이 있으면 함수내부(지역)변수를 우선으로 인식하고
	this-> 를 명시해줘서 구조체의 멤버변수를 가리킬 수 있게함.

	프로그래밍 언어학자들(교수)은 포인터 표현을 안보여주려고함
	C는 운영체제를 만들기위해 반드시 포인터가 필요했었는데
	C++에서 다시 숨겨버림

	int g_a;
	int m_a;		// this-> 를 설명안하려고 접두어를 붙이게됨 m_
	int a
	교육자: 유지보수/관리를 핑계

	왜 이런 문법이 생겼는가...?
*/