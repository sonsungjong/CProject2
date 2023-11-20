import "HeaderUnits.h";

int main()
{
	const size_t number_of_elements = 10;
	std::unique_ptr<int[]> values = std::make_unique<int[]>(number_of_elements);

	for (int index = 0; index < number_of_elements; ++index)
	{
		values[index] = index;
	}

	values.get()[9] = 99;
	for (int index = 0; index < number_of_elements; ++index)
	{
		printf("%d ", values.get()[index]);
	}

	return 0;
}