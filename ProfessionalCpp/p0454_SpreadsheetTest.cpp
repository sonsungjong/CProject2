import spreadsheet;
import <vector>;

Spreadsheet createObject()
{
	return Spreadsheet { 3,2 };
}

int main0454()
{
	std::vector<Spreadsheet> vec;
	for (size_t i = 0; i < 2; ++i) {
		printf("Iteration %d\n", i);
		vec.push_back(Spreadsheet{ 100, 100 });
		printf("\n");
	}

	Spreadsheet s(2, 3);
	s = createObject();

	Spreadsheet s2(5, 6);
	s2 = s;

}