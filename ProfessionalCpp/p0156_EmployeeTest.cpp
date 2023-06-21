import <iostream>;
import employee0156;

using namespace std;
using namespace Records;

int main0156e() {
	printf("Testing the Employee class\n");
	Employee156 emp{ "Jane", "Doe" };
	emp.setFirstName("John");
	emp.setLastName("Doe");
	emp.setEmployeeNumber(71);
	emp.setSalary(50'000);
	emp.promote();
	emp.promote(50);
	emp.hire();
	emp.display();

	return 0;
}