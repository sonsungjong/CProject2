import <iostream>;

class Base506
{
public:
	// virtual 을 붙인 메서드만 오버라이딩이 가능하다.
	virtual void someMethod() 
	{
		printf("This is Base's version of someMethod\n");
	}
	virtual void func1() {}
	virtual void func2() {}
	void nonVirtualFunc() {}

	Base506() {}
	// final 클래스를 제외하고는 소멸자는 항상 virtual 을 붙인다
	virtual ~Base506() = default;
	// 5의 법칙 (소멸자, 복제 생성자, 복사 대입 연산자, 이동 생성자, 이동 대입 연산자)
	Base506(const Base506&) = default;										// 복사 생성자
	Base506& operator=(const Base506&) = default;						// 복사 대입 연산자
	Base506(Base506&&) noexcept = default;								// 이동 생성자
	Base506& operator=(Base506&&) noexcept = default;				// 이동 대입 연산자

protected:
	int m_protectedInt{ 0 };
private:
	int m_privateInt{ 0 };
};

// 상속
class Derived506 final : public Base506
{
	// final을 클래스명 오른쪽에 붙이면 상속 방지를 부여한다.
public:
	// 오버라이딩을 하면 메서드 옆에 override 를 붙인다. (+베이스 클래스에서 virtual을 붙였으면 파생 클래스에서도 자동적으로 virtual 속성이 들어간다)
	void someMethod() override 
	{
		printf("This is Derived's version of someMethod\n");
	}

	virtual void someOtherMethod() 
	{
		printf("This is someOtherMethod\n");
	}

	virtual void func2() override final {}				// 메서드에 final 키워드를 부여하면 더이상 오버라이딩이 불가능해진다 (부모의 virtual을 물려받을 때 더이상 오버라이딩 금지시키기 용도)
	void nonVirtualFunc() {}				// 부모에 virtual이 없었으니깐 새로 생성한 것임(오버라이딩X)
	
	Derived506() {
		m_string = new char[30] {0};
		printf("m_string allocated\n");
	}

	// 5의 법칙 (소멸자, 복제 생성자, 복사 대입 연산자, 이동 생성자, 이동 대입 연산자)
	virtual ~Derived506() {
		delete[] m_string;
		printf("m_string deallocated\n");
	}																									// 소멸자 : 객체가 파괴될 때 호출되어 자원을 해제한다
	Derived506(const Derived506&) = default;										// 복사 생성자 : 객체를 동일한 타입의 다른 객체로부터 복사 생성할 때 호출된다
	Derived506& operator=(const Derived506&) = default;						// 복사 대입 연산자 : 객체를 동일한 타입의 다른 객체에 복사 대입할 때 호출된다
	Derived506(Derived506&&) noexcept = default;								// 이동 생성자 : 객체를 동일한 타입의 임시 객체로부터 이동 생성할 때 호출된다
	Derived506& operator=(Derived506&&) noexcept = default;				// 이동 대입 연산자 : 객체를 동일한 타입의 임시 객체에 이동 대입할 때 호출된다

private:
	char* m_string;
};

int main506()
{
	Derived506 myDerived;
	myDerived.someMethod();
	myDerived.someOtherMethod();

	// 다형성을 적용할 땐 반드시 포인터나 레퍼런스 객체로 선언해야한다
	Base506* base = new Derived506;				// Derived 객체를 생성해서 Base 포인터에 저장한다.
	Base506& ref = myDerived;						// 베이스 래퍼런스
	
	ref.someMethod();

	delete base;				// 부모의 소멸자를 virtual로 선언해야만 자식의 소멸자가 호출된다

	return 0;
}