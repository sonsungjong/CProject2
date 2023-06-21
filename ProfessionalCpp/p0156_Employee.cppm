export module employee0156;
import <string>;
namespace Records{
	const int DefaultStartingSalary { 30'000 };									// 외부에서 접근할 일이 없는 (기본급여)
	export const int DefaultRaiseAndDemeritAmount { 1'000 };			// 외부에서 접근할 수 있도록 export (승진/좌천 액수)

	export class Employee156
	{
	public:
		Employee156(const std::string& firstName, const std::string& lastName);				// 성씨와 이름을 받는 생성자

		void promote(int raiseAmount = DefaultRaiseAndDemeritAmount);
		void demote(int demeritAmount = DefaultRaiseAndDemeritAmount);
		void hire();				// 고용 또는 재고용
		void fire();					// 해고
		void display() const;				// 정보 출력

		void setFirstName(const std::string& firstName);
		const std::string& getFirstName() const;

		void setLastName(const std::string& lastName);
		const std::string& getLastName() const;

		void setEmployeeNumber(int employeeNumber);
		int getEmployeeNumber() const;

		void setSalary(int newSalary);
		int getSalary() const;

		bool isHired() const;

	private:
		std::string m_firstName;
		std::string m_lastName;
		int m_employeeNumber { -1 };
		int m_salary { DefaultStartingSalary };
		bool m_hired { false };
	};

}
