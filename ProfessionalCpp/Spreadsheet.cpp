module spreadsheet;
#include <stdexcept>
#include <format>

// 생성자
Spreadsheet::Spreadsheet(size_t width, size_t height)
	: m_width(width), m_height(height)
{
	m_cells = new SpreadsheetCell*[m_width];
	for (size_t i = 0; i < m_width; ++i) {
		m_cells[i] = new SpreadsheetCell[m_height];
	}
}

// 소멸자 (소멸자는 익셉션을 던져서는 안된다)
Spreadsheet::~Spreadsheet()
{
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
