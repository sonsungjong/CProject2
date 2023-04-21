import "HeaderUnits.h";
#include <limits>

// 숫자 경계값
int main0071()
{
	int num = INT_MAX;				// 정수의 경계값

	// numeric_limits<>
	std::cout << "int:" << std::endl;
	printf("Max int value: %d\n", std::numeric_limits<int>::max());
	printf("Min int value: %d\n", std::numeric_limits<int>::min());
	printf("Lowest int value: %d\n", std::numeric_limits<int>::lowest());

	std::cout << "\ndouble:\n";
	printf("Max double value: %e\n", std::numeric_limits<double>::max());				// %e 지수표기법
	printf("Min double value: %e\n", std::numeric_limits<double>::min());
	printf("Lowest double value: %e\n", std::numeric_limits<double>::lowest());

}
