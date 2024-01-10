module spreadsheet;
#include <stdexcept>
#include <format>

// 생성자
Spreadsheet::Spreadsheet(size_t width, size_t height)
	: m_width(width), m_height(height)			// 위임생성자
{
	// 생성자에서 동적할당
	m_cells = new SpreadsheetCell*[m_width];
	for (size_t i = 0; i < m_width; ++i) {
		m_cells[i] = new SpreadsheetCell[m_height];
	}
}

// 소멸자 (소멸자는 익셉션을 던져서는 안된다)
Spreadsheet::~Spreadsheet()
{
	// 소멸자에서 동적할당 해제
	for (size_t i = 0; i < m_width; ++i) {
		delete[] m_cells[i];				// 내부배열 정리하고
	}
	delete[] m_cells;					// 바깥 배열 정리하고
	m_cells = nullptr;				// nullptr로 대입
}

void Spreadsheet::setCellAt(size_t x, size_t y, const SpreadsheetCell& cell)
{
	verifyCoordinate(x, y);
	m_cells[x][y] = cell;
}

SpreadsheetCell& Spreadsheet::getCellAt(size_t x, size_t y)
{
	verifyCoordinate(x, y);
	return m_cells[x][y];
}

// 복제생성자 정의 (멤버변수에 동적할당이 필요하면 필수구현)
Spreadsheet::Spreadsheet(const Spreadsheet& src)
	: Spreadsheet(src.m_width, src.m_height)			// 위임 생성자
{
	// 깊은 복사로 대입한다
	for (size_t i = 0; i < m_width; ++i) {
		for (size_t j = 0; j < m_height; ++j) {
			m_cells[i][j] = src.m_cells[i][j];
		}
	}
}

// 대입연산자 정의 (멤버변수에 동적할당이 필요하면 필수구현)
Spreadsheet& Spreadsheet::operator=(const Spreadsheet& rhs)
{
	// 자신을 대입하는지 확인한다
	if (this == &rhs) {
		return *this;
	}

	// 기존 메모리를 해제한다
	for (size_t i = 0; i < m_width; ++i) {
		delete[] m_cells[i];
	}
	delete[] m_cells;
	m_cells = nullptr;

	// 메모리를 새로 할당한다
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_cells = new SpreadsheetCell*[m_width];
	for (size_t i = 0; i < m_width; ++i) {
		m_cells[i] = new SpreadsheetCell[m_height];
	}

	// 데이터를 복제한다
	for (size_t i = 0; i < m_width; ++i) {
		for (size_t j = 0; j < m_height; ++j) {
			m_cells[i][j] = rhs.m_cells[i][j];
		}
	}

	return *this;
}


bool Spreadsheet::inRange(size_t value, size_t upper) const
{
	if (value >= upper) {
		return false;
	}
	else {
		return true;
	}
}

void Spreadsheet::verifyCoordinate(size_t x, size_t y) const
{
	if (x >= m_width) {
		throw std::out_of_range(std::format("{} must be less than {}.", x, m_width));
	}
	if (y >= m_height) {
		throw std::out_of_range(std::format("{} must be less than {}.", y, m_height));
	}
}
