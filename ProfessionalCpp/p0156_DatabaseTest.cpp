import <iostream>;
import database0156;

using namespace std;
using namespace Records;

int main0156d()
{
	Database myDB;
	Employee156& emp1{ myDB.addEmployee("Greg", "Wallis") };
	emp1.fire();

	Employee156& emp2{ myDB.addEmployee("Marc", "White") };
	emp2.setSalary(100'000);

	Employee156& emp3{ myDB.addEmployee("John", "Doe") };
	emp3.promote();

	printf("all employees: \n\n");
	myDB.displayAll();

	printf("\ncurrent employees: \n\n");
	myDB.displayCurrent();

	printf("\nformer employees: \n\n");
	myDB.displayFormer();

	return 0;
}