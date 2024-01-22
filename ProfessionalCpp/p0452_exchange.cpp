//import "HeaderUnits.h";
#include <iostream>
#include <format>
#include <utility>

int main452()
{
	int a = 11;
	int b = 22;
	std::cout << std::format("Before std::exchange() : a = {}, b = {}", a, b) << std::endl;

	int returnedValue = std::exchange(a, b);
	std::cout << std::format("After exchange(): a = {}, b = {}", a, b) << std::endl;
	std::cout << std::format("exchange() returned() : {}", returnedValue) << std::endl;

	return 0;
}