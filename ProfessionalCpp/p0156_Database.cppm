export module database0156;
import <string>;
import <vector>;
import <stdexcept>;
import <exception>;
import employee0156;

namespace Records {
	const int FirstEmployeeNumber { 1'000 };

	export class Database
	{
	public:
		Employee156& addEmployee(const std::string& firstName, const std::string& lastName);
		Employee156& getEmployee(int employeeNumber);
		Employee156& getEmployee(const std::string& firstName, const std::string& lastName);

		void displayAll() const;
		void displayCurrent() const;
		void displayFormer() const;

	private:
		std::vector<Employee156> m_employees;
		int m_nextEmployeeNumber { FirstEmployeeNumber };
	};
}
