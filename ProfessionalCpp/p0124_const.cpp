import "HeaderUnits.h";

// �Ű����� const
void func0124(const std::string* some_string)
{
	// some_string�� ������ �� ����
}

class Airline0124
{
public:
	// const �޼���
	int getNumberOfMiles() const;					// ������ ����� ������ �� ���� �Ѵ�

private:
	int m_number_of_miles{0};
};


int main0124()
{
	// C������ ������� ���� ����� #define���� ��������� C++������ const�� ����Ѵ�
	// ���� ������� �ʵ��� �����Ϸ��� ó���Ѵ�
	
	// const ���
	const int number{ 2 };
	
	// const ������
	const int* pointer1;
	int* const pointer2 { nullptr };
	const int* const pointer3 { nullptr };


	return 0;
}