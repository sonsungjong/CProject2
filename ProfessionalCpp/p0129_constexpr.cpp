import "HeaderUnits.h";

//const int getSize() { return 32; }			// 안됨
constexpr int getSize() { return 32; }					// constexpr 함수는 constexpr 함수만 호출할 수 있다

consteval double inchToMm(double inch) { return inch * 25.4; }

class Rect0129
{
public:
	// constexpr 생성자
	constexpr Rect0129(size_t width, size_t height) : m_width{ width }, m_height{ height } {}
	constexpr size_t getArea() const { return m_width * m_height; }
private:
	size_t m_width{ 0 }, m_height{ 0 };
};

int main0129()
{
	// constexpr : 상수표현식, 컴파일 시간에 평가되는 표현식
	int arr[getSize() +1];

	//constexpr Rect rect{ 8,2 };
	//int arr[rect.getArea()];

	return 0;
}