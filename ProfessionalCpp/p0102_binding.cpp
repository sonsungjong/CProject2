import "HeaderUnits.h";
#include <array>

struct Point0102
{
	double m_x;
	double m_y;
	double m_z;
};

int main0102()
{
	std::array values { 11, 22, 33 };				// 배열

	// 구조적 바인딩을 사용하려면 자료형을 auto 로 명시해야한다
	auto [x1, y1, z1] {values};

	Point0102 point;
	point.m_x = 1.0;
	point.m_y = 2.0;
	point.m_z = 3.0;

	auto [x, y, z] {point};

	return 0;
}