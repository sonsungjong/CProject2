import "HeaderUnits.h";

#include <stdexcept>

double divideNumbers(double numerator, double denominator)
{
	if (denominator == 0) {
		throw std::invalid_argument("Denominator cannot be 0");
		// return ���� �ʰ� ������ ��� �ߴ�
	}
	return numerator / denominator;
}

// �и� 0�� �� ����
int main0147()
{
	printf("10�� 0���� ������ \n");
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