import <iostream>;
import <string>;
import <string_view>;
import <optional>;
import <charconv>;
import <vector>;

class SpreadsheetCell541
{
public:
	virtual ~SpreadsheetCell541() = default;
	virtual void set(std::string_view value) = 0;				// ���� ���� �޼��� (������ ����, �ش� Ŭ������ �߻� Ŭ������ �ȴ�, �Ļ�Ŭ�����θ� ��ü�� ������ �� �ְ� �ȴ�)
	virtual std::string getString() const = 0;					// ���� ���� �޼���

	// ��������� �Ļ�Ŭ������
};

class DoubleSpreadsheetCell541 : public SpreadsheetCell541
{
public:
	void set(std::string_view value) override
	{
		m_value = stringToDouble(value);
	}

	virtual void set(double value)
	{
		m_value = value;
	}

	std::string getString() const override
	{
		// ���� ���� �ִ��� Ȯ���ϰ� ����
		return (m_value.has_value() ? doubleToString(m_value.value()) : "");
	}

private:
	static std::string doubleToString(double value)
	{
		return std::to_string(value);
	}

	static double stringToDouble(std::string_view value)
	{
		double number = 0;
		std::from_chars(value.data(), value.data() + value.size(), number);
		return number;
	}

	std::optional<double> m_value;					// �������� �ְ� �������� �ִ� optional (���ܹ߻� ����)
};

class StringSpreadsheetCell541 : public SpreadsheetCell541
{
public:
	StringSpreadsheetCell541() = default;				// ������ ������
	StringSpreadsheetCell541(const DoubleSpreadsheetCell541& cell)
		: m_value{ cell.getString() }
	{}

	void set(std::string_view value) override
	{
		m_value = value;
	}

	std::string getString() const override
	{
		return m_value.value_or("");				// NULL ����
	}

private:
	std::optional<std::string> m_value;
};

StringSpreadsheetCell541 operator+(const StringSpreadsheetCell541& lhs, const StringSpreadsheetCell541& rhs)
{
	StringSpreadsheetCell541 newCell;
	newCell.set(lhs.getString() + rhs.getString());
	return newCell;
}

int main541()
{
	std::unique_ptr<SpreadsheetCell541> cell  { new StringSpreadsheetCell541{}};

	std::vector<std::unique_ptr<SpreadsheetCell541>> cellArray;
	cellArray.push_back(std::make_unique<StringSpreadsheetCell541>());
	cellArray.push_back(std::make_unique<StringSpreadsheetCell541>());
	cellArray.push_back(std::make_unique<DoubleSpreadsheetCell541>());

	cellArray[0]->set("hello");
	cellArray.at(1)->set("10");
	cellArray[2]->set("18");
	
	printf("Vector: [%s,%s,%s]\n", cellArray[0]->getString().c_str(), cellArray[1]->getString().c_str(), cellArray[2]->getString().c_str());

	DoubleSpreadsheetCell541 myDbl;
	myDbl.set(8.4);
	StringSpreadsheetCell541 result{ myDbl + myDbl };
	printf("%s\n", result.getString().c_str());

	return 0;
}