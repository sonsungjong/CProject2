module spreadsheet;
#include <stdexcept>
#include <format>
#include <utility>

// 생성자
Spreadsheet::Spreadsheet(size_t width, size_t height)
	: m_width(width), m_height(height)			// 위임생성자
{
	printf("Normal constructor\n");
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
	printf("Copy constructor\n");
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
	Spreadsheet temp(rhs);				// 복제
	swap(temp);								// 현재 객체와 맞바꾸기

	return *this;
}

void Spreadsheet::swap(Spreadsheet& other) noexcept
{
	std::swap(m_width, other.m_width);
	std::swap(m_height, other.m_height);
	std::swap(m_cells, other.m_cells);
}

// 이동 의미론 구현 : 이동 생성자
Spreadsheet::Spreadsheet(Spreadsheet&& src) noexcept
{
	printf("Move constructor\n");
	moveFrom(src);
}

// 이동 의미론 구현 : 이동 대입 연산자
Spreadsheet& Spreadsheet::operator=(Spreadsheet&& rhs) noexcept
{
	// 자기 자신을 대입하는지 확인한다
	if (this == &rhs) {
		return *this;
	}

	// 예전 메모리를 해제한다
	cleanup();
	moveFrom(rhs);
	return *this;
}

// 이동 의미론 구현 : 소멸자와 이동 대입 연산자에서 사용
void Spreadsheet::cleanup() noexcept
{
	for (size_t i = 0; i < m_width; ++i) {
		delete[] m_cells[i];
	}
	delete[] m_cells;
	m_cells = nullptr;
	m_width = m_height = 0;
}

// 이동 의미론 구현 : 원본 객체의 멤버 변수를 대상 객체로 이동시킨 뒤 원본 객체를 리셋
void Spreadsheet::moveFrom(Spreadsheet& src) noexcept
{
	// 데이터에 대한 얕은 복제
	m_width = src.m_width;
	m_height = src.m_height;
	m_cells = src.m_cells;

	// 소유권이 이전되었기 때문에 소스 객체를 리셋한다
	src.m_width = 0;
	src.m_height = 0;
	src.m_cells = nullptr;

	// std::exchange() 사용
	//m_width = std::exchange(src.m_width, 0);
	//m_height = std::exchange(src.m_height, 0);
	//m_cells = std::exchange(src.m_cells, nullptr);

	// 객체 데이터 멤버는 std::move() 로 이동
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
	T temp = std::move(a);				// T가 복제하기 너무 큰 경우 이동 의미론을 적용해서 복제가 발생하지 않게 구현한다
	a = std::move(b);
	b = std::move(temp);
}
