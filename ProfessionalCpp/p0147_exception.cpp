import "HeaderUnits.h";

#include <stdexcept>

double divideNumbers(double numerator, double denominator)
{
	if (denominator == 0) {
		throw std::invalid_argument("Denominator cannot be 0");
		// return 하지 않고 실행을 즉시 중단
	}
	return numerator / denominator;
}

// 분모가 0일 때 예외
int main0147()
{
	printf("10을 0으로 나누기 \n");
	try {
		printf("%lf \n", divideNumbers(2.5, 0.5));
		printf("%lf \n", divideNumbers(2.3, 0));
		printf("%lf \n", divideNumbers(4.5, 2.5));
	}
	catch (const std::invalid_argument& exception) {
		std::cout << "Exception caught: " << exception.what() << std::endl;
	}

	return 0;
}