import "HeaderUnits.h";
#include <array>

int main0098()
{
	std::array<int, 3> arr {9, 8, 7};
	std::cout << std::format("Array size = {}", arr.size()) << std::endl;
	std::cout << std::format("2nd element = {}", arr[1]) << std::endl;

	return 0;
}