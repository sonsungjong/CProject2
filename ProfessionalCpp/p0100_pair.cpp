import "HeaderUnits.h";
#include <utility>

int main0100()
{
	// std::pair
	// �� ���� �ϳ��� ���´�
	// first �� second�� ������ �� �ִ�
	std::pair<double, int> my_pair {1.23, 5};
	std::cout << std::format("{} {}", my_pair.first, my_pair.second);

	return 0;
}