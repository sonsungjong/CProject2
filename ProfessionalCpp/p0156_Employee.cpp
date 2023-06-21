module employee0156;
import <iostream>;
import <format>;
using namespace std;

namespace Records {
	Employee156::Employee156(const string& firstName, const string& lastName) : m_firstName{ firstName }, m_lastName{ lastName }
	{
	}

	void Employee156::promote(int raiseAmount)
	{
		setSalary(m_salary + raiseAmount);
	}

	void Employee156::demote(int demeritAmount)
	{
		setSalary(m_salary - demeritAmount);
	}

	void Employee156::hire()
	{
		m_hired = true;
	}

	void Employee156::fire()
	{
		m_hired = false;
	}

	bool Employee156::isHired() const
	{
		return m_hired;
	}

	/*
		현재 직원에 대한 정보를 화면에 표시
	*/
	void Employee156::display() const
	{
		cout << format("Employee: {}, {}", m_lastName, m_firstName) << endl;
		printf("----------------------------------------------- \n");
		printf("%s\n", (m_hired ? "Current Employee" : "Former Employee"));
		cout << format("Employee Number : {}", m_employeeNumber) << endl;
		cout << format("Salary: ${}\n\n", m_salary);
	}

	void Employee156::setFirstName(const string& firstName)
	{
		m_firstName = firstName;
	}

	const string& Employee156::getFirstName() const
	{
		return m_firstName;
	}

	void Employee156::setLastName(const std::string& lastName)
	{
		m_lastName = lastName;
	}

	const std::string& Employee156::getLastName() const
	{
		return m_lastName;
	}

	void Employee156::setEmployeeNumber(int employeeNumber)
	{
		m_employeeNumber = employeeNumber;
	}

	int Employee156::getEmployeeNumber() const
	{
		return m_employeeNumber;
	}

	void Employee156::setSalary(int newSalary)
	{
		m_salary = newSalary;
	}

	int Employee156::getSalary() const
	{
		return m_salary;
	}

}