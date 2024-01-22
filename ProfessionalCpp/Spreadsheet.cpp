module spreadsheet;
#include <stdexcept>
#include <format>
#include <utility>

// ������
Spreadsheet::Spreadsheet(size_t width, size_t height)
	: m_width(width), m_height(height)			// ���ӻ�����
{
	printf("Normal constructor\n");
	// �����ڿ��� �����Ҵ�
	m_cells = new SpreadsheetCell*[m_width];
	for (size_t i = 0; i < m_width; ++i) {
		m_cells[i] = new SpreadsheetCell[m_height];
	}
}

// �Ҹ��� (�Ҹ��ڴ� �ͼ����� �������� �ȵȴ�)
Spreadsheet::~Spreadsheet()
{
	// �Ҹ��ڿ��� �����Ҵ� ����
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

// ���������� ���� (��������� �����Ҵ��� �ʿ��ϸ� �ʼ�����)
Spreadsheet::Spreadsheet(const Spreadsheet& src)
	: Spreadsheet(src.m_width, src.m_height)			// ���� ������
{
	printf("Copy constructor\n");
	// ���� ����� �����Ѵ�
	for (size_t i = 0; i < m_width; ++i) {
		for (size_t j = 0; j < m_height; ++j) {
			m_cells[i][j] = src.m_cells[i][j];
		}
	}
}

// ���Կ����� ���� (��������� �����Ҵ��� �ʿ��ϸ� �ʼ�����)
Spreadsheet& Spreadsheet::operator=(const Spreadsheet& rhs)
{
	Spreadsheet temp(rhs);				// ����
	swap(temp);								// ���� ��ü�� �¹ٲٱ�

	return *this;
}

void Spreadsheet::swap(Spreadsheet& other) noexcept
{
	std::swap(m_width, other.m_width);
	std::swap(m_height, other.m_height);
	std::swap(m_cells, other.m_cells);
}

// �̵� �ǹ̷� ���� : �̵� ������
Spreadsheet::Spreadsheet(Spreadsheet&& src) noexcept
{
	printf("Move constructor\n");
	moveFrom(src);
}

// �̵� �ǹ̷� ���� : �̵� ���� ������
Spreadsheet& Spreadsheet::operator=(Spreadsheet&& rhs) noexcept
{
	// �ڱ� �ڽ��� �����ϴ��� Ȯ���Ѵ�
	if (this == &rhs) {
		return *this;
	}

	// ���� �޸𸮸� �����Ѵ�
	cleanup();
	moveFrom(rhs);
	return *this;
}

// �̵� �ǹ̷� ���� : �Ҹ��ڿ� �̵� ���� �����ڿ��� ���
void Spreadsheet::cleanup() noexcept
{
	for (size_t i = 0; i < m_width; ++i) {
		delete[] m_cells[i];
	}
	delete[] m_cells;
	m_cells = nullptr;
	m_width = m_height = 0;
}

// �̵� �ǹ̷� ���� : ���� ��ü�� ��� ������ ��� ��ü�� �̵���Ų �� ���� ��ü�� ����
void Spreadsheet::moveFrom(Spreadsheet& src) noexcept
{
	// �����Ϳ� ���� ���� ����
	m_width = src.m_width;
	m_height = src.m_height;
	m_cells = src.m_cells;

	// �������� �����Ǿ��� ������ �ҽ� ��ü�� �����Ѵ�
	src.m_width = 0;
	src.m_height = 0;
	src.m_cells = nullptr;

	// std::exchange() ���
	//m_width = std::exchange(src.m_width, 0);
	//m_height = std::exchange(src.m_height, 0);
	//m_cells = std::exchange(src.m_cells, nullptr);

	// ��ü ������ ����� std::move() �� �̵�
	//m_name = std::move(src.m_name);
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

void swap(Spreadsheet& first, Spreadsheet& second) noexcept
{
	first.swap(second);
}

template<typename T>
void Spreadsheet::swapCopy(T& a, T& b)
{
	T temp = std::move(a);				// T�� �����ϱ� �ʹ� ū ��� �̵� �ǹ̷��� �����ؼ� ������ �߻����� �ʰ� �����Ѵ�
	a = std::move(b);
	b = std::move(temp);
}
