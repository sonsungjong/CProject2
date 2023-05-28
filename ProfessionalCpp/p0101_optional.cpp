import "HeaderUnits.h";
#include <optional>

std::optional<int> GetData(bool giveIt)
{
	if (giveIt) {
		return 42;
	}
	return std::nullopt;
}

int main()
{
	std::optional<int> data1 { GetData(true) };
	std::optional<int> data2 { GetData(false) };

	cout << "data1.has_value = " << data1.has_value() << endl;

	if (data2) {
		cout << "data2 has a value." << endl;
	}

	return 0;
}