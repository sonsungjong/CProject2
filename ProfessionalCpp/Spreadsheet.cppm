export module spreadsheet;
//#include <cstddef>

export import spreadsheet_cell;

export class Spreadsheet
{
public:
	Spreadsheet(size_t width, size_t height);
	~Spreadsheet();
	void setCellAt(size_t x, size_t y, const SpreadsheetCell& cell);
	SpreadsheetCell& getCellAt(size_t x, size_t y);

	// 클래스 멤버변수에 동적할당 변수가 있으므로 복제생성자와 대입연산자를 직접 구현해줘야한다
	Spreadsheet(const Spreadsheet& src);								// 복제생성자
	Spreadsheet& operator=(const Spreadsheet& rhs);			// 대입연산자
	
	void swap(Spreadsheet& other) noexcept;

	// 대입과 값 전달방식 금지
	//Spreadsheet(const Spreadsheet& src) = delete;
	//Spreadsheet& operator=(const Spreadsheet& rhs) = delete;

	// 이동 의미론 구현
	Spreadsheet(Spreadsheet&& src) noexcept;									// 이동 생성자
	Spreadsheet& operator=(Spreadsheet&& rhs) noexcept;				// 이동 대입 연산자
	void cleanup() noexcept;
	void moveFrom(Spreadsheet& src) noexcept;

	template<typename T>
	void swapCopy(T& a, T& b);

private:
	bool inRange(size_t value, size_t upper) const;
	void verifyCoordinate(size_t x, size_t y) const;
	size_t m_width { 0 };
	size_t m_height { 0 };
	SpreadsheetCell** m_cells { nullptr };				// 동적할당 메모리
};

export void swap(Spreadsheet& first, Spreadsheet& second) noexcept;