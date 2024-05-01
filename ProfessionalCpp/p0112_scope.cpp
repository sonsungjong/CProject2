import "HeaderUnits.h";

class Demo
{
public:
	int get() { return 5; }
};

int get() { return 10; }

namespace NS
{
	int get() { return 20; }
}

int main0112()
{
	Demo d;
	std::cout << d.get() << std::endl;				// 5
	std::cout << NS::get() << std::endl;			// 20
	std::cout << ::get() << std::endl;				// 10
	std::cout << get() << std::endl;					// 10

	return 0;
}
