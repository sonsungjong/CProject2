import "HeaderUnits.h";
#include <cmath>

int main0072()
{
	// 영 초기화 zero initialization
	float myFloat{};					// 0.0f
	int myInt{};						// 0
	int* p_int{};					// nullptr
	// 객체는 디폴트 생성자로 초기화

	// 캐스트
	myFloat = 3.14;
	int i1{ (int)myFloat };					// C 형태의 캐스트
	int i2{ int(myFloat) };
	int i3{ static_cast<int>(myFloat) };					// C++ 권장

	short someShort{};
	long someLong{ someShort };				// 강제 형변환 (short가 long으로)
	
	// 부동소수점 수
	// std::isnan();				// 숫자가 아님 Not a Number
	// std::isinf();					// 무한인지 검사
	printf("%e", std::numeric_limits<double>::infinity);

	// 연산자
	int someInteger{ 256 };
	float someFloat{};
	double someDouble{};

	someInteger++;
	someInteger *= 2;
	someShort = static_cast<short>(someInteger);
	someLong = someShort * 10000;
	someFloat = someLong + 0.785f;
	someDouble = static_cast<double>(someFloat) / 100000;

	std::cout << someDouble << std::endl;

	int num1{ 34 + 8 * 2 + 21 / 7 % 2 };

	int num2{ 34 + (8 * 2) + ((21 / 7) % 2) };				// 작은 표현식이나 소괄호를 사용
}
