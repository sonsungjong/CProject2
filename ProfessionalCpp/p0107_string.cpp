import "HeaderUnits.h";

int main0107()
{
	std::string myString{ "Hello, World" };
	std::cout << std::format("The value of myString is {}", myString) << std::endl;
	std::cout << std::format("The second letter is {}", myString[1]) << std::endl;

	return 0;
}
