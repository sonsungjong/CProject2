import "HeaderUnits.h";
// 아토믹 연산

int main1294()
{
	// 1. 락-프리 동시성 데이터 구조 : 락을 사용하지 않고 데이터 구조를 동시에 접근하게 만들기위한 연산
	// 2. fetch_add 사용
	std::atomic<int> value = 10;
	std::cout << "Value = " << value << "\n";				// 10
	int fetched = value.fetch_add(4);					// value에 4를 더하고 이전값은 리턴한다
	std::cout << "Fetched = " << fetched << "\n";			// 10
	std::cout << "Value = " << value << "\n";					// 14

	// fetch_add(), fetch_sub(), fetch_and(), fetch_or(), fetch_xor(), ++, --, -=, &=, ^=, !=, ...

	printf("\n\n\n");

	// C++20 아토믹 스마트 포인터
	std::atomic<std::shared_ptr<int>> num = std::make_shared<int>(10);
	auto num2 = std::make_shared<int>(20);

	std::shared_ptr<int> old = num.load();
	std::cout << "Initial value: " << *old << std::endl;

	// 원자적으로 num 업데이트 시도
	if (num.compare_exchange_strong(old, num2))					// num이 old와 같다면 num2를 대입한다
	{
		std::cout << "num was exchanged, new value: " << *num.load() << std::endl;
	}
	else {
		std::cout << "Exchange failed, current value: " << *num.load() << std::endl;
	}
	

	return 0;
}