import <iostream>;

import <format>;
import <string>;
import database0156;
import employee0156;

using namespace std;
using namespace Records;

int displayMenu();
void doHire(Database& db);
void doFire(Database& db);
void doPromote(Database& db);

int main0156main()
{
	Database employeeDB;
	bool done { false };

	while (!done) {
		int selection { displayMenu() };
		switch (selection) {
		case 0:
			done = true;
			break;
		case 1:
			doHire(employeeDB);
			break;
		case 2:
			doFire(employeeDB);
			break;
		case 3:
			doPromote(employeeDB);
			break;
		case 4:
			employeeDB.displayAll();
			break;
		case 5:
			employeeDB.displayCurrent();
			break;
		case 6:
			employeeDB.displayFormer();
			break;
		default:
			cerr << "Unknown command." << endl;
			break;
		}
	}

	return 0;
}

int displayMenu()
{
	int selection;
	printf("\nEmployee Database\n--------------------------------------\n");
	printf("1) Hire a new employee\n");
	printf("2) Fire an employee\n");
	printf("3) Promote an employee\n");
	printf("4) List all employees\n");
	printf("5) List all current employees\n");
	printf("6) List all former employees\n");
	printf("0) Quit\n\n---> ");
	scanf_s("%d", &selection);

	return selection;
}

void doHire(Database& db)
{
	string firstName;
	string lastName;

	printf("First name? ");
	cin >> firstName;

	printf("Last name? ");
	cin >> lastName;

	auto& employee { db.addEmployee(firstName, lastName) };
	cout << format("Hired employee {} {} with employee number {}.", firstName, lastName, employee.getEmployeeNumber()) << endl;
}

void doFire(Database& db)
{
	int employeeNumber = -1;
	printf("Employee number? ");
	cin >> employeeNumber;

	try {
		auto& emp { db.getEmployee(employeeNumber) };
		emp.fire();
		printf("Employee %d terminated.\n", employeeNumber);
	}
	catch (const std::logic_error& exception) {
		cerr << format("Unable to terminate employee: {}", exception.what()) << endl;
	}
}

void doPromote(Database& db)
{
	int employeeNumber = -1;
	printf("Employee number? ");
	cin >> employeeNumber;

	int raiseAmount = -1;
	printf("How much of a raise? ");
	cin >> raiseAmount;

	try {
		auto& emp { db.getEmployee(employeeNumber) };
		emp.promote(raiseAmount);
	}
	catch (const std::logic_error& exception) {
		cerr << format("Unable to promote employee: {}", exception.what()) << endl;
	}
}
