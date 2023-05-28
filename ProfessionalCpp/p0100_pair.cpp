import "HeaderUnits.h";
#include <utility>

int main0100()
{
	// std::pair
	// 두 값을 하나로 묶는다
	// first 와 second로 접근할 수 있다
	std::pair<double, int> my_pair {1.23, 5};
	std::cout << std::format("{} {}", my_pair.first, my_pair.second);

	return 0;
}