module database0156;

using namespace std;

namespace Records {
	Employee156& Database::addEmployee(const string& firstName, const string& lastName)
	{
		Employee156 theEmployee { firstName, lastName };
		theEmployee.setEmployeeNumber(m_nextEmployeeNumber++);
		theEmployee.hire();
		m_employees.push_back(theEmployee);
		return m_employees.back();
	}

	Employee156& Database::getEmployee(int employeeNumber)
	{
		for (auto& employee : m_employees) {
			if (employee.getEmployeeNumber() == employeeNumber) {
				return employee;
			}
		}
		throw logic_error{"No employee found"};
	}

	Employee156& Database::getEmployee(const std::string& firstName, const std::string& lastName)
	{
		for (auto& employee : m_employees) {
			if (employee.getFirstName() == firstName && employee.getLastName() == lastName) {
				return employee;
			}
		}
		throw logic_error{"No employee found"};
	}

	void Database::displayAll() const
	{
		for (const auto& employee : m_employees) { employee.display(); }
	}

	void Database::displayCurrent() const
	{
		for (const auto& employee : m_employees) {
			if (employee.isHired()) { employee.display(); }
		}
	}

	void Database::displayFormer() const
	{
		for (const auto& employee : m_employees) {
			if (!employee.isHired()) { employee.display(); }
		}
	}
}