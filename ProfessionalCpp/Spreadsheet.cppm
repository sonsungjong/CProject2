module;
#include <cstddef>

export module spreadsheet;

export import spreadsheet_cell;
import <memory>;

export class SpreadsheetApplication {};

export class Spreadsheet
{
public:
	// 중첩 클래스 (전방 선언)
	class Cell;

	//Spreadsheet(size_t width = MaxWidth, size_t height = MaxHeight);
	Spreadsheet(const SpreadsheetApplication& theApp, size_t width = MaxWidth, size_t height = MaxHeight);
	~Spreadsheet();

	// 클래스 멤버변수에 동적할당 변수가 있으므로 복제생성자와 대입연산자를 직접 구현해줘야한다
	Spreadsheet(const Spreadsheet& src);								// 복제생성자
	Spreadsheet(Spreadsheet&&) noexcept;					// 이동 생성자
	Spreadsheet& operator=(const Spreadsheet& rhs);			// 대입연산자
	Spreadsheet& operator=(Spreadsheet&&) noexcept;				// 이동 대입 연산자

	void setCellAt(size_t x, size_t y, const SpreadsheetCell& cell);
	SpreadsheetCell& getCellAt(size_t x, size_t y);				// 디폴트인수는 오른쪽부터 차례로 정의할 수 있다.
	const SpreadsheetCell& getCellAt(size_t x, size_t y) const;				// const를 통해서도 오버로딩할 수 있다
	size_t getId() const;
	
	void swap(Spreadsheet& other) noexcept;

	// 대입과 값 전달방식 금지
	//Spreadsheet(const Spreadsheet& src) = delete;

	// 이동 의미론 구현
	void cleanup() noexcept;
	void moveFrom(Spreadsheet& src) noexcept;

	template<typename T>
	void swapCopy(T& a, T& b);


	static const size_t MaxHeight = 100;				// 클래스 내부에서만 사용할 상수는 글로벌 상수보다는 static const 멤버를 사용하자(public으로)
	static const size_t MaxWidth = 100;



private:
	bool inRange(size_t value, size_t upper) const;
	void verifyCoordinate(size_t x, size_t y) const;
	size_t m_width { 0 };
	size_t m_height { 0 };
	SpreadsheetCell** m_cells { nullptr };				// 동적할당 메모리

	static size_t ms_count;
	static inline size_t mss_count = 0;

	size_t m_id = 0;
	const SpreadsheetApplication& m_theApp;
};

export void swap(Spreadsheet& first, Spreadsheet& second) noexcept;

// 중첩클래스 전방 선언시
class Spreadsheet::Cell
{
public:
	Cell() = default;
	Cell(double initialValue);
private:
	double m_value;
};