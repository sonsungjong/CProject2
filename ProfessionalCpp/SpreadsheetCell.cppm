export module spreadsheet_cell;
import <string>;
import <string_view>;
import <vector>;
import <charconv>;

export class SpreadsheetCell
{
public:
	//SpreadsheetCell(const SpreadsheetCell& src) = delete;			// 복제 생성자 삭제
	SpreadsheetCell(const SpreadsheetCell& src) = default;			// 복제 생성자 (명시)
	SpreadsheetCell(double initialValue);
	explicit SpreadsheetCell(std::string_view initialValue);				// 암묵적 변환이 필요한 경우가 아니라면 단일 매개변수를 받는 생성자는 explicit 으로 지정한다
	SpreadsheetCell() = default;
	SpreadsheetCell& operator=(const SpreadsheetCell& rhs);				// 디폴트 대입 연산자

	~SpreadsheetCell();				// 소멸자

	void setValue(double value);
	double getValue() const;

	void setString(std::string_view inString);
	std::string getString() const;

private:
	std::string doubleToString(double value) const;
	double stringToDouble(std::string_view inString) const;
	double m_value { 0 };

};
