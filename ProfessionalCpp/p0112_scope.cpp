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
	cout << d.get() << endl;				// 5
	cout << NS::get() << endl;			// 20
	cout << ::get() << endl;				// 10
	cout << get() << endl;					// 10

	return 0;
}
