import "HeaderUnits.h";
import employee;

using namespace std;

int main0080()
{
	// ���� ���ڵ� ���� �� �� ä���
	Employee anEmployee;
	anEmployee.firstInitial = 'J';
	anEmployee.lastInitial = 'D';
	anEmployee.employeeNumber = 42;
	anEmployee.salary = 80000;

	// ���� ���ڵ忡 ����� �� ���
	cout << format("Employee: {} {}", anEmployee.firstInitial, anEmployee.lastInitial) << endl;
	cout << format("number: {}", anEmployee.employeeNumber) << endl;
	cout << format("Salary: ${}", anEmployee.salary) << endl;

	return 0;
}