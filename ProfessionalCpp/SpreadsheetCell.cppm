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

	enum class Color {Red=1, Green, Blue, Yellow};
	void setColor(Color color);
	Color getColor() const;

	SpreadsheetCell add(const SpreadsheetCell& cell) const;					// 두개의 셀을 더한 뒤 제3의 셀을 리턴 (원본 셀을 변경하지 않기 때문에 메서드를 const로 선언)
	SpreadsheetCell operator+(const SpreadsheetCell& cell) const;				// add -> +
	SpreadsheetCell operator+(double rhs) const;										// 임시 객체 생성으로 생성자 선택 과정에서 성능이 떨어지는 것을 방지하기위해 명시적으로 선언
	SpreadsheetCell operator+=(const SpreadsheetCell& rhs);					// 기존 객체를 변경하고 수정된 객체에 대한 래퍼런스를 생성한다
	SpreadsheetCell operator-=(const SpreadsheetCell& rhs);
	SpreadsheetCell operator*=(const SpreadsheetCell& rhs);
	SpreadsheetCell operator/=(const SpreadsheetCell& rhs);

	[[nodiscard]] std::partial_ordering operator<=>(const SpreadsheetCell&) const = default;
	[[nodiscard]] bool operator==(const SpreadsheetCell&) const = default;
	//[[nodiscard]] bool operator==(double rhs) const;
	//[[nodiscard]] std::partial_ordering operator<=>(double rhs) const;


private:
	static std::string doubleToString(double value);				// (static은 구현코드에서는 생략 가능)
	static double stringToDouble(std::string_view inString);
	double m_value { 0 };

	mutable size_t m_numAccesses = 0;				// mutable로 선언하면 const메서드에서도 수정이 가능하다

	Color m_color { Color::Red };
};

// 전역함수를 정의하려면 인터페이스 파일에서 선언하고 익스포트 해야한다.
export SpreadsheetCell operator+(const SpreadsheetCell& lhs, const SpreadsheetCell& rhs);
export SpreadsheetCell operator/(const SpreadsheetCell& lhs, const SpreadsheetCell& rhs);
