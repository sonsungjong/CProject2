import "HeaderUnits.h";

int main0107()
{
	string myString{ "Hello, World" };
	cout << format("The value of myString is {}", myString) << endl;
	cout << format("The second letter is {}", myString[1]) << endl;

	return 0;
}
