import "HeaderUnits.h";

// 매개변수 const
void func0124(const std::string* some_string)
{
	// some_string을 변경할 수 없음
}

class Airline0124
{
public:
	// const 메서드
	int getNumberOfMiles() const;					// 데이터 멤버를 수정할 수 없게 한다

private:
	int m_number_of_miles{0};
};


int main0124()
{
	// C언어에서는 변경되지 않을 상수는 #define으로 사용했지만 C++에서는 const로 대신한다
	// 값이 변경되지 않도록 컴파일러가 처리한다
	
	// const 상수
	const int number{ 2 };
	
	// const 포인터
	const int* pointer1;
	int* const pointer2 { nullptr };
	const int* const pointer3 { nullptr };


	return 0;
}