import "HeaderUnits.h";
// ����� ����

int main1294()
{
	// 1. ��-���� ���ü� ������ ���� : ���� ������� �ʰ� ������ ������ ���ÿ� �����ϰ� ��������� ����
	// 2. fetch_add ���
	std::atomic<int> value = 10;
	std::cout << "Value = " << value << "\n";				// 10
	int fetched = value.fetch_add(4);					// value�� 4�� ���ϰ� �������� �����Ѵ�
	std::cout << "Fetched = " << fetched << "\n";			// 10
	std::cout << "Value = " << value << "\n";					// 14

	// fetch_add(), fetch_sub(), fetch_and(), fetch_or(), fetch_xor(), ++, --, -=, &=, ^=, !=, ...

	printf("\n\n\n");

	// C++20 ����� ����Ʈ ������
	std::atomic<std::shared_ptr<int>> num = std::make_shared<int>(10);
	auto num2 = std::make_shared<int>(20);

	std::shared_ptr<int> old = num.load();
	std::cout << "Initial value: " << *old << std::endl;

	// ���������� num ������Ʈ �õ�
	if (num.compare_exchange_strong(old, num2))					// num�� old�� ���ٸ� num2�� �����Ѵ�
	{
		std::cout << "num was exchanged, new value: " << *num.load() << std::endl;
	}
	else {
		std::cout << "Exchange failed, current value: " << *num.load() << std::endl;
	}
	

	return 0;
}