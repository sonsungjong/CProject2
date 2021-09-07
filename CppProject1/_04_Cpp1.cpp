#include <iostream>
// 상속
using namespace std;

int add(int a, int b) {
	return a + b;
}	// C언어를 사용하다보면 기능을 조금 추가할때가 있음
// 이때 절댓값으로 a,b를 더해야할 필요가 있고 기존 add함수는 유지해야한다면

int addEx(int a, int b) {
	if (a < 0) a = -a;
	return a + b;
}	// 이렇게 기능이 조금 추가되면 함수를 하나 더 만들 수 밖에 없음(복붙으로 비슷한 함수 많아짐)
// 이때 printf()로 확인이 필요해지면 모든 함수에 각각 추가를 해줘야함
// 중복코드의 문제
// C++은 클래스 단위로 중복까지 일어나게됨

class MyClass 
{
private:
	int b = 0;
protected:
	// private이지만 상속받은 클래스는 가능
	int a = 0;
	char* mp;
public:
	void getA(int a) {
		this->a = a;
	}
};
class MyClassEx : public MyClass 
{
	// 컴파일러에 의해 MyClass가 복사됨
	// 완전히 일치하지 않기 때문에 상속받음
public:
	void getA(int a)	// 변경(오버라이딩) 이름과 함수원형 동일
	{
		cout << a << endl;
		this->a = a;		// MyClass::getA(a); 로 대체가능
	}
	int add(int a, int b) {	// 추가
		return a + b;
	}
};
// 오버로딩 : 함수의 이름중복허용
// 오버라이딩 : 상속에서 부모클래스에 있는 함수의 기능을 변경하는 것

int main상속() {
	MyClassEx mc;
	
	int g = 0;
	int* pg = &g;
	*pg = *pg + 2;
	cout << g << endl;
	
	return 0;
}

/*
	클래스 중 일부기능 수정요구하고 함수명변경X, 어떤사람은 변경불필요
	-> 클래스 복붙 후 새로운 클래스생성 -> 중복범위 엄청 커짐

	한 프로그램을 적용하는 현장에 따라 조금씩 수정을 해줘야하는 경우가 많음
	적용현장별로 클래스를 만들게되면 버그수정시 모든 소스를 수정해야함
	월화수목금금금...

	커피자판기 -> 녹차자판기 : 버그발생시 둘다 수정
	
	->컴파일러에게 만들 클래스에 비슷한 다른 클래스코드를 복붙하게함
	개발자가 복붙하면 수정이 어려워지지만 컴파일러에게 복붙하게하면 관리필요x
	(눈에만 안보이고 코드는 복붙됨)
	class MyString{};
	class MyStringEx : public MyString{};	상속
	: public 클래스명
*/