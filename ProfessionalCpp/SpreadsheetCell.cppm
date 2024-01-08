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
	double getValue() const;

	void setString(std::string_view inString);
	std::string getString() const;

private:
	std::string doubleToString(double value) const;
	double stringToDouble(std::string_view inString) const;
	double m_value { 0 };

};
