import "HeaderUnits.h";
#include <optional>

std::optional<int> GetData(bool giveIt)
{
	if (giveIt) {
		return 42;
	}
	return std::nullopt;
}

int main0101()
{
	std::optional<int> data1 { GetData(true) };
	std::optional<int> data2 { GetData(false) };

	cout << "data1.has_value = " << data1.has_value() << endl;

	if (data2) {
		cout << "data2 has a value." << endl;
	}

	cout << "data1.value = " << data1.value() << endl;
	cout << "data1.value = " << *data1 << endl;
	cout << "data2.value = " << data2.value_or(0) << endl;

	return 0;
}