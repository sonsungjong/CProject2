import "HeaderUnits.h";
#include <cstddef>

int main0068()
{
	123;					// ������
	0173;				// 8����
	0x7B;				// 16����
	0b1111011;			// 2����

	23'456'789;							// �ڸ��� ������
	0.123'456f;
	
	// ���� �ʱ�ȭ
	int i{ -7 };
	short s{ 16 };
	long l{ -7L };
	long long ll{ 14LL };

	unsigned int ui{ 2U };
	unsigned short us{ 23U };
	unsigned long ul{ 5400UL };
	unsigned long long ull{ 140ULL };

	float f{ 7.2f };
	double d{ 7.2 };
	long double dl{ 16.98L };

	char ch{ 'm' };
	wchar_t w{ L'm' };
	char8_t c8{ u8'm' };
	char16_t c16{ u'm' };
	char32_t c32{ U'm' };

	bool b{ true };

	// char ����Ʈ ��ü��
	std::byte by{ 42 };

	printf("%d \n", i);

	return 0;
}

