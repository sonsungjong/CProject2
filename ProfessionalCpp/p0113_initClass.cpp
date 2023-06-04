import "HeaderUnits.h";

struct CircleStruct0113
{
	int x, y;
	double radius;
};

class CircleClass0113
{
public:
	CircleClass0113(int x, int y, double radius) : m_x{ x }, m_y{ y }, m_radius{ radius }
	{}

private:
	int m_x, m_y;
	double m_radius;
};

struct Employee0113
{
	char firstInitial;
	char lastInitial;
	short employeeNumber;
	int salary;
};

class MyClass0113
{
public:
	MyClass0113() : m_array{ 0,1,2,3 } 
	{
		// 생성자로 배열 초기화
	}

private:
	int m_array[4];
};

int main0113()
{
	CircleStruct0113 myCircle1 = { 10, 10, 2.5 };
	CircleClass0113 myCircle2(10, 10, 2.5);
	Employee0113 employee {'J', 'D', 42, 80'000};

	int* p_array = new int[4] {0, 1, 2, 3};

	// C++20 부터는 배열크기를 생략할 수 있다
	int* p_array2 = new int[] {0, 1, 2, 3};

	// C++20 지정 초기자
	Employee0113 emp{
		.firstInitial = 'J',
		.lastInitial = 'D',
		.employeeNumber = 42,
		.salary = 80'000
		// 새로운 멤버가 추가되어도 상관없음
	};

	return 0;
}

