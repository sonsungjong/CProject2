#include <iostream>

using namespace std;

struct Tips
{
	int from_year;
	int member_count;
};

struct Tipsware
{
	int from_year;
	int member_count;
	int sales;
};		// 8byte가 일치

struct A{
	int a, b, z;
};

struct B{
	int c, d;
};

int main42() {
	Tips data1;
	data1.from_year = 1992;
	data1.member_count = 8;
	// 자료형이 2개면 코드도 2개

	Tipsware data2;
	data2.from_year = 2002;
	data2.member_count = 6;
	data2.sales = 6000;
	// 자료형이 3개면 코드도 3개

	Tips* p1 = &data1;
	p1->from_year = 1992;
	p1->member_count = 8;

	Tipsware* p2 = &data2;
	p2->from_year = 2002;
	p2->member_count = 6;
	p2->sales = 6000;

	// 이름이 달라도 구조만 같으면 접근가능
	Tipsware data3;
	Tips* p3 = (Tips*)&data3;
	p3->from_year = 2002;			// Tipsware의 4byte 메모리에 2002 할당 (0~4)
	p3->member_count = 6;		// Tipsware의 4byte 메모리에 6 할당		(5~8)
	*(&p3->member_count+1) = 5000;		// Tipsware의 4byte 메모리에 5000 할당		(9~12)

	A dataA;

	B* p_data = (B*)&dataA;		// 형변환	(byte기준으로 구조가 동일하기 때문)
	p_data->c = 4;		// dataA.a 에 4가 들어감
	*(&p_data->c+1) = 5;		// dataA.b 에 5 들어감
	p_data->d = 99;		// dataA.b 에 5 들어감
	*(&p_data->c+2) = 77;		// dataA.z 에 77 들어감
	
	cout << dataA.a << endl;
	cout << dataA.b << endl;
	cout << dataA.z << endl;

	// A >= B* : 안전, 넘어간 부분은 사용X
	// A <= B* : 위험, 메모리를 벗어남(메모리 침범)
	// 포인터보다 대상의 자료형이 더 커야 안전함

	// 그러므로, 상속에서 자식이 더 크므로 부모포인터로 자식객체를 안전하게 가리킬 수 있음
	// 다형성 : 부모클래스 포인터로 자식클래스를 가리킬 수 있다

	return 0;
}

/*
	다형성 : 부모클래스의 포인터로 자신과 자식클래스들을 가리킬 수 있기 때문에 부모클래스포인터로 기능구현을 전부 할 수 있다.
*/