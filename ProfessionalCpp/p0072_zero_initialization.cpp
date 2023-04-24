import "HeaderUnits.h";
#include <cmath>

int main0072()
{
	// �� �ʱ�ȭ zero initialization
	float myFloat{};					// 0.0f
	int myInt{};						// 0
	int* p_int{};					// nullptr
	// ��ü�� ����Ʈ �����ڷ� �ʱ�ȭ

	// ĳ��Ʈ
	myFloat = 3.14;
	int i1{ (int)myFloat };					// C ������ ĳ��Ʈ
	int i2{ int(myFloat) };
	int i3{ static_cast<int>(myFloat) };					// C++ ����

	short someShort{};
	long someLong{ someShort };				// ���� ����ȯ (short�� long����)
	
	// �ε��Ҽ��� ��
	// std::isnan();				// ���ڰ� �ƴ� Not a Number
	// std::isinf();					// �������� �˻�
	printf("%e", std::numeric_limits<double>::infinity);

	// ������
	int someInteger{ 256 };
	float someFloat{};
	double someDouble{};

	someInteger++;
	someInteger *= 2;
	someShort = static_cast<short>(someInteger);
	someLong = someShort * 10000;
	someFloat = someLong + 0.785f;
	someDouble = static_cast<double>(someFloat) / 100000;

	std::cout << someDouble << std::endl;

	int num1{ 34 + 8 * 2 + 21 / 7 % 2 };

	int num2{ 34 + (8 * 2) + ((21 / 7) % 2) };				// ���� ǥ�����̳� �Ұ�ȣ�� ���
}
