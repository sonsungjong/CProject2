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

	enum class Color {Red=1, Green, Blue, Yellow};
	void setColor(Color color);
	Color getColor() const;

	SpreadsheetCell add(const SpreadsheetCell& cell) const;					// �ΰ��� ���� ���� �� ��3�� ���� ���� (���� ���� �������� �ʱ� ������ �޼��带 const�� ����)
	SpreadsheetCell operator+(const SpreadsheetCell& cell) const;				// add -> +
	SpreadsheetCell operator+(double rhs) const;										// �ӽ� ��ü �������� ������ ���� �������� ������ �������� ���� �����ϱ����� ��������� ����
	SpreadsheetCell operator+=(const SpreadsheetCell& rhs);					// ���� ��ü�� �����ϰ� ������ ��ü�� ���� ���۷����� �����Ѵ�
	SpreadsheetCell operator-=(const SpreadsheetCell& rhs);
	SpreadsheetCell operator*=(const SpreadsheetCell& rhs);
	SpreadsheetCell operator/=(const SpreadsheetCell& rhs);

	[[nodiscard]] std::partial_ordering operator<=>(const SpreadsheetCell&) const = default;
	[[nodiscard]] bool operator==(const SpreadsheetCell&) const = default;
	//[[nodiscard]] bool operator==(double rhs) const;
	//[[nodiscard]] std::partial_ordering operator<=>(double rhs) const;


private:
	static std::string doubleToString(double value);				// (static�� �����ڵ忡���� ���� ����)
	static double stringToDouble(std::string_view inString);
	double m_value { 0 };

	mutable size_t m_numAccesses = 0;				// mutable�� �����ϸ� const�޼��忡���� ������ �����ϴ�

	Color m_color { Color::Red };
};

// �����Լ��� �����Ϸ��� �������̽� ���Ͽ��� �����ϰ� �ͽ���Ʈ �ؾ��Ѵ�.
export SpreadsheetCell operator+(const SpreadsheetCell& lhs, const SpreadsheetCell& rhs);
export SpreadsheetCell operator/(const SpreadsheetCell& lhs, const SpreadsheetCell& rhs);
