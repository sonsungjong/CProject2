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
	std::array values { 11, 22, 33 };				// �迭

	// ������ ���ε��� ����Ϸ��� �ڷ����� auto �� ����ؾ��Ѵ�
	auto [x1, y1, z1] {values};

	Point0102 point;
	point.m_x = 1.0;
	point.m_y = 2.0;
	point.m_z = 3.0;

	auto [x, y, z] {point};

	return 0;
}