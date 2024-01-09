module spreadsheet;
#include <stdexcept>
#include <format>

// ������
Spreadsheet::Spreadsheet(size_t width, size_t height)
	: m_width(width), m_height(height)
{
	m_cells = new SpreadsheetCell*[m_width];
	for (size_t i = 0; i < m_width; ++i) {
		m_cells[i] = new SpreadsheetCell[m_height];
	}
}

// �Ҹ��� (�Ҹ��ڴ� �ͼ����� �������� �ȵȴ�)
Spreadsheet::~Spreadsheet()
{
	for (size_t i = 0; i < m_width; ++i) {
		delete[] m_cells[i];				// ���ι迭 �����ϰ�
	}
	delete[] m_cells;					// �ٱ� �迭 �����ϰ�
	m_cells = nullptr;				// nullptr�� ����
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
