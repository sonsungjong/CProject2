import "HeaderUnits.h";

//const int getSize() { return 32; }			// �ȵ�
constexpr int getSize() { return 32; }					// constexpr �Լ��� constexpr �Լ��� ȣ���� �� �ִ�

consteval double inchToMm(double inch) { return inch * 25.4; }

class Rect0129
{
public:
	// constexpr ������
	constexpr Rect0129(size_t width, size_t height) : m_width{ width }, m_height{ height } {}
	constexpr size_t getArea() const { return m_width * m_height; }
private:
	size_t m_width{ 0 }, m_height{ 0 };
};

int main0129()
{
	// constexpr : ���ǥ����, ������ �ð��� �򰡵Ǵ� ǥ����
	int arr[getSize() +1];

	//constexpr Rect rect{ 8,2 };
	//int arr[rect.getArea()];

	return 0;
}