import "HeaderUnits.h";
#include <array>
#include <initializer_list>

int makeSum(initializer_list<int> values)
{
	int total{ 0 };
	for (int value : values) {
		total += value;
	}
	return total;
}

int main0103()
{
	// while
	int i{ 0 };
	while (i < 5) {
		printf("This is while. \n");
		++i;
	}

	// do~while
	int j{ 100 };
	do {
		printf("This is do~while \n");
		++i;
	} while (i < 5);

	// for
	for (int i{ 0 }; i < 5; ++i) {
		printf("This is for \n");
	}

	// for (range)
	std::array arr {1, 2, 3, 4};
	for (int i : arr) {
		printf("%d\n", i);
	}

	// initializer_list
	int a{ makeSum({1,2,3}) };
	int b{ makeSum({10, 20, 30, 40, 50, 60}) };
	

	return 0;
}