module;
#include <cstddef>

export module spreadsheet;

export import spreadsheet_cell;
import <memory>;

export class SpreadsheetApplication {};

export class Spreadsheet
{
public:
	// ��ø Ŭ���� (���� ����)
	class Cell;

	//Spreadsheet(size_t width = MaxWidth, size_t height = MaxHeight);
	Spreadsheet(const SpreadsheetApplication& theApp, size_t width = MaxWidth, size_t height = MaxHeight);
	~Spreadsheet();

	// Ŭ���� ��������� �����Ҵ� ������ �����Ƿ� ���������ڿ� ���Կ����ڸ� ���� ����������Ѵ�
	Spreadsheet(const Spreadsheet& src);								// ����������
	Spreadsheet(Spreadsheet&&) noexcept;					// �̵� ������
	Spreadsheet& operator=(const Spreadsheet& rhs);			// ���Կ�����
	Spreadsheet& operator=(Spreadsheet&&) noexcept;				// �̵� ���� ������

	void setCellAt(size_t x, size_t y, const SpreadsheetCell& cell);
	SpreadsheetCell& getCellAt(size_t x, size_t y);				// ����Ʈ�μ��� �����ʺ��� ���ʷ� ������ �� �ִ�.
	const SpreadsheetCell& getCellAt(size_t x, size_t y) const;				// const�� ���ؼ��� �����ε��� �� �ִ�
	size_t getId() const;
	
	void swap(Spreadsheet& other) noexcept;

	// ���԰� �� ���޹�� ����
	//Spreadsheet(const Spreadsheet& src) = delete;

	// �̵� �ǹ̷� ����
	void cleanup() noexcept;
	void moveFrom(Spreadsheet& src) noexcept;

	template<typename T>
	void swapCopy(T& a, T& b);


	static const size_t MaxHeight = 100;				// Ŭ���� ���ο����� ����� ����� �۷ι� ������ٴ� static const ����� �������(public����)
	static const size_t MaxWidth = 100;



private:
	bool inRange(size_t value, size_t upper) const;
	void verifyCoordinate(size_t x, size_t y) const;
	size_t m_width { 0 };
	size_t m_height { 0 };
	SpreadsheetCell** m_cells { nullptr };				// �����Ҵ� �޸�

	static size_t ms_count;
	static inline size_t mss_count = 0;

	size_t m_id = 0;
	const SpreadsheetApplication& m_theApp;
};

export void swap(Spreadsheet& first, Spreadsheet& second) noexcept;

// ��øŬ���� ���� �����
class Spreadsheet::Cell
{
public:
	Cell() = default;
	Cell(double initialValue);
private:
	double m_value;
};