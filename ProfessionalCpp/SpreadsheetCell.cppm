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
	void setValue(std::string_view value);				// 오버로딩 (매개변수의 타입이나 갯수가 다르다)
	void setValue(int) = delete;								// 오버로딩 (정수로는 대입하지 못하게 한다)

	double getValue() const;				// 멤버 변수를 변경하지 않는 메서드 const

	void setString(std::string_view inString);
	std::string getString() const;				// 멤버 변수를 변경하지 않는 메서드 const (const는 프로토타입의 일부분이기 때문에 구현코드에서도 적어줘야한다)

private:
	static std::string doubleToString(double value);				// (static은 구현코드에서는 생략 가능)
	static double stringToDouble(std::string_view inString);
	double m_value { 0 };

	mutable size_t m_numAccesses = 0;				// mutable로 선언하면 const메서드에서도 수정이 가능하다
};
