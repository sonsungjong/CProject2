#include <iostream>

struct A {
	int a;
	int b;
};
struct B {
	int c;		// A와 일치, 위치 일치
	int d;		// A와 일치, 위치 일치
	int m;	// 더 커도 됨
};

struct Tips {
	int from_year;
	int member_count;
	// ㅁㅁㅁㅁㅇㅇㅇㅇ
};
// 비슷한 구조
struct Tipsware {
	int from_year;
	int member_count;
	int sales;
	// ㅁㅁㅁㅁㅇㅇㅇㅇㅂㅂㅂㅂ
};

class Taps 
{
public:
	int m_from_year;
	int m_member_count;
};

class Tapsware : public Taps
{
public:
	int m_sales;
	// 포인터를 사용할땐 상속한 부모포인터를 사용(다형성)
	// Taps* 로 사용
};

int main다형성() 
{
	A data = {2, 3};
	A temp = {4 ,5};
	// 모든 변수 복사
	data.a = temp.a;
	data.b = temp.b;
	// 모든 변수 복사
	memcpy(&data, &temp, sizeof(A));
	// 모든 변수 복사
	data = temp;		// 직접 대입하면 문제가 발생할 수 있어 복사생성자를 사용
							// (동적메모리 할당있을때)
	
	// 객체가 생성될때 생성자가 호출된다.

	// 구조만 맞으면 하나의 포인터로 여러개의 구조체에 접근할 수 있음
	B* p0 = (B*)&data;		// 형변환
	p0->c = 34;		// A구조체의 data객체의 a변수 값을 바꿔버림
	std::cout << data.a << std::endl;		// 구조만 맞으면

	Tips data1;
	data1.from_year = 1992;
	data1.member_count = 9;

	Tipsware data2;
	data2.from_year = 2002;
	data2.member_count = 6;
	data2.sales = 8000;

	Tips* p1 = &data1;
	Tipsware* p2 = &data2;
	p1->from_year = 1992;
	p1->member_count = 5;

	p2->from_year = 2002;
	p2->member_count = 7;
	p2->sales = 8888;

	Tips* p3 = (Tips*)&data2;
	p3->from_year = 3003;
	p3->member_count = 600;
	*(&p3->member_count + 1) = 500;	// Tipsware 3번째 변수에 넣어버림
	
	return 0;
}


// = 동적할당 메모리가 없을 경우 대입연산자로 사용
// = 동적할당 메모리가 있을 경우 복사생성자로 사용됨

// 다형성
// 
// 기능을 구현할때 자료형을 최대한 적게 쓰는 것이 유지보수에 유리하다 => 다형성 등장
// 한개의 자료형만 써서 프로그래밍
// 하나의 형태가 여러가지를 커버

// 다형성 : 부모 포인터로 자식클래스를 가리킬 수 있다