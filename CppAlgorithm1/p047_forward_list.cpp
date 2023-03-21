#include <string>
#include <iostream>
#include <forward_list>

struct citizen
{
	std::string name;
	int age;
};

std::ostream& operator<<(std::ostream& os, const citizen& c)
{
	return (os << "[" << c.name << ", " << c.age << "]");
}

int main047()
{
	std::forward_list<citizen> citizens = {
		{"Kim", 22}, {"Lee", 25}, {"Park", 18}, {"Jin", 16}
	};

	auto citizens_copy = citizens;

	printf("전체 시민들: ");
	for (const auto& c : citizens) {
		std::cout << c << " ";
	}
	printf("\n");

	// 나이를 기준으로 투표권이 없으면 삭제
	citizens.remove_if([](const citizen& c) {
		return (c.age < 19);				// 19보다 작으면 리스트에서 제거
	});

	printf("투표권이 있는 시민들: ");
	for (const auto& c : citizens) {
		std::cout << c << " ";
	}
	printf("\n");

	// 내년에 투표권이 생기는 사람들
	citizens_copy.remove_if([](const citizen& c) {return (c.age != 18); });
	printf("내년에 투표권이 생기는 시민들: ");
	for (const auto& c : citizens_copy) {
		std::cout << c << " ";
	}
	printf("\n");


	return 0;
}