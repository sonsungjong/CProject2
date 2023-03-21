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

	printf("��ü �ùε�: ");
	for (const auto& c : citizens) {
		std::cout << c << " ";
	}
	printf("\n");

	// ���̸� �������� ��ǥ���� ������ ����
	citizens.remove_if([](const citizen& c) {
		return (c.age < 19);				// 19���� ������ ����Ʈ���� ����
	});

	printf("��ǥ���� �ִ� �ùε�: ");
	for (const auto& c : citizens) {
		std::cout << c << " ";
	}
	printf("\n");

	// ���⿡ ��ǥ���� ����� �����
	citizens_copy.remove_if([](const citizen& c) {return (c.age != 18); });
	printf("���⿡ ��ǥ���� ����� �ùε�: ");
	for (const auto& c : citizens_copy) {
		std::cout << c << " ";
	}
	printf("\n");


	return 0;
}