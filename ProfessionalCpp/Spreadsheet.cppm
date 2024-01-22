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

	// Ŭ���� ��������� �����Ҵ� ������ �����Ƿ� ���������ڿ� ���Կ����ڸ� ���� ����������Ѵ�
	Spreadsheet(const Spreadsheet& src);								// ����������
	Spreadsheet& operator=(const Spreadsheet& rhs);			// ���Կ�����
	
	void swap(Spreadsheet& other) noexcept;

	// ���԰� �� ���޹�� ����
	//Spreadsheet(const Spreadsheet& src) = delete;
	//Spreadsheet& operator=(const Spreadsheet& rhs) = delete;

	// �̵� �ǹ̷� ����
	Spreadsheet(Spreadsheet&& src) noexcept;									// �̵� ������
	Spreadsheet& operator=(Spreadsheet&& rhs) noexcept;				// �̵� ���� ������
	void cleanup() noexcept;
	void moveFrom(Spreadsheet& src) noexcept;

	template<typename T>
	void swapCopy(T& a, T& b);

private:
	bool inRange(size_t value, size_t upper) const;
	void verifyCoordinate(size_t x, size_t y) const;
	size_t m_width { 0 };
	size_t m_height { 0 };
	SpreadsheetCell** m_cells { nullptr };				// �����Ҵ� �޸�
};

export void swap(Spreadsheet& first, Spreadsheet& second) noexcept;