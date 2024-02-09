import <iostream>;

class Something529
{
public:
	Something529()
	{
		printf("2");
	}
	virtual ~Something529()
	{
		printf("2");
	}
};

class Base529
{
public:
	Base529(int i)					// 디폴트 생성자 제거 (상속받은 곳에서 초기자를 통해 매개변수를 대입해줘야한다 // 멤버변수 대입 절대금지)
	{
		printf("1");
	}
	virtual ~Base529()
	{
		printf("1");
	}
};

class Derived529 : public Base529
{
public:
	Derived529()
		: Base529(7)				// 부모 생성자 초기화
	{
		printf("3");
	}
	virtual ~Derived529()
	{
		printf("3");
	}

private:
	Something529 m_data;
	int m_num;				// 멤버변수는 부모생성자 호출 이후에 선언되기 때문에 부모 생성자에 전달할 수 없다.
};

int main529()
{
	Derived529 obj;				// 123
	// 부모 먼저 생성 -> 멤버변수 생성 -> 생성자 호출

	// 321
	// 소멸자 본문 실행 -> 생성과 반대 순서로 멤버변수 제거 -> 부모 제거

	return 0;
}