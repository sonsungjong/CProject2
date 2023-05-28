import "HeaderUnits.h";
#include <vector>

int main0099()
{
	// 크기가 동적으로 변하는 배열 : vector
	std::vector<int> my_vector {11, 22};

	// push_back 으로 추가
	my_vector.push_back(33);
	my_vector.push_back(44);

	std::cout << std::format("1st element: {}", my_vector[0]) << std::endl;
	std::cout << std::format("2nd element: {}", my_vector[1]) << std::endl;
	std::cout << std::format("3rd element: {}", my_vector[2]) << std::endl;
	std::cout << std::format("4th element: {}", my_vector[3]) << std::endl;

	return 0;
}