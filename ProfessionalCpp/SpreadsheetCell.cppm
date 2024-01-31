export module spreadsheet_cell;
import <string>;
import <string_view>;
import <vector>;
import <charconv>;

export class SpreadsheetCell
{
public:
	//SpreadsheetCell(const SpreadsheetCell& src) = delete;			// ���� ������ ����
	SpreadsheetCell(const SpreadsheetCell& src) = default;			// ���� ������ (���)
	SpreadsheetCell(double initialValue);
	explicit SpreadsheetCell(std::string_view initialValue);				// �Ϲ��� ��ȯ�� �ʿ��� ��찡 �ƴ϶�� ���� �Ű������� �޴� �����ڴ� explicit ���� �����Ѵ�
	SpreadsheetCell() = default;
	SpreadsheetCell& operator=(const SpreadsheetCell& rhs);				// ����Ʈ ���� ������

	~SpreadsheetCell();				// �Ҹ���

	void setValue(double value);
	void setValue(std::string_view value);				// �����ε� (�Ű������� Ÿ���̳� ������ �ٸ���)
	void setValue(int) = delete;								// �����ε� (�����δ� �������� ���ϰ� �Ѵ�)

	double getValue() const;				// ��� ������ �������� �ʴ� �޼��� const

	void setString(std::string_view inString);
	std::string getString() const;				// ��� ������ �������� �ʴ� �޼��� const (const�� ������Ÿ���� �Ϻκ��̱� ������ �����ڵ忡���� ��������Ѵ�)

private:
	static std::string doubleToString(double value);				// (static�� �����ڵ忡���� ���� ����)
	static double stringToDouble(std::string_view inString);
	double m_value { 0 };

	mutable size_t m_numAccesses = 0;				// mutable�� �����ϸ� const�޼��忡���� ������ �����ϴ�
};
