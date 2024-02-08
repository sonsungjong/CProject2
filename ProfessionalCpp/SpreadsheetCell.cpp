module spreadsheet_cell;

import <iostream>;
using namespace std;

SpreadsheetCell::~SpreadsheetCell()
{
	printf("Destructor called.\n");
}

void SpreadsheetCell::setValue(double value)
{
	m_value = value;
}

void SpreadsheetCell::setValue(std::string_view value)
{
	// �����ε�
	m_value = stringToDouble(value);
}

double SpreadsheetCell::getValue() const
{
	return m_value;
}

void SpreadsheetCell::setString(std::string_view value) 
{
	m_value = stringToDouble(value);
}

string SpreadsheetCell::getString() const
{
	return doubleToString(m_value);
}

void SpreadsheetCell::setColor(Color color)
{
	m_color = color;
}

SpreadsheetCell::Color SpreadsheetCell::getColor() const
{
	return m_color;
}

SpreadsheetCell SpreadsheetCell::add(const SpreadsheetCell& cell) const
{
	return SpreadsheetCell{getValue() + cell.getValue()};
}

SpreadsheetCell SpreadsheetCell::operator+(const SpreadsheetCell& cell) const
{
	return SpreadsheetCell{getValue() + cell.getValue()};
}

SpreadsheetCell SpreadsheetCell::operator+(double rhs) const
{
	return SpreadsheetCell(getValue() + rhs);
}

SpreadsheetCell SpreadsheetCell::operator+=(const SpreadsheetCell& rhs)
{
	setValue(getValue() + rhs.getValue());
	return *this;
}

SpreadsheetCell SpreadsheetCell::operator-=(const SpreadsheetCell& rhs)
{
	setValue(getValue() - rhs.getValue());
	return *this;
}

SpreadsheetCell SpreadsheetCell::operator*=(const SpreadsheetCell& rhs)
{
	setValue(getValue() * rhs.getValue());
	return *this;
}

SpreadsheetCell SpreadsheetCell::operator/=(const SpreadsheetCell& rhs)
{
	if (rhs.getValue() != 0) {
		setValue(getValue() / rhs.getValue());
	}
	else {
		throw invalid_argument{ "Divided by zero." };
	}
	return *this;
}

string SpreadsheetCell::doubleToString(double value)
{
	return to_string(value);
}

double SpreadsheetCell::stringToDouble(std::string_view value)
{
	double number { 0 };
	from_chars(value.data(), value.data() + value.size(), number);
	return number;
}

SpreadsheetCell::SpreadsheetCell(double initialValue) 
{
	setValue(initialValue);
}

SpreadsheetCell::SpreadsheetCell(std::string_view initialValue)
	: SpreadsheetCell {stringToDouble(initialValue)}
{
	//setString(initialValue);				// ���ӻ����ڷ� ��ü (�����ڿ��� �ٸ� ������ ȣ��)
}

SpreadsheetCell& SpreadsheetCell::operator=(const SpreadsheetCell& rhs)
{
	if (this == &rhs) 
	{
		// �ڱ� �ڽ��� ������ ���
		return *this;
	}
	else {
		m_value = rhs.m_value;					// ���� ����
		return *this;
	}
}

//SpreadsheetCell::SpreadsheetCell()
//{
//	m_value = 0;
//}

// �����Լ� (�Ű����� ��ġ ����ȭ)
SpreadsheetCell operator+(const SpreadsheetCell& lhs, const SpreadsheetCell& rhs)
{
	return SpreadsheetCell{ lhs.getValue() + rhs.getValue() };
}

SpreadsheetCell operator/(const SpreadsheetCell& lhs, const SpreadsheetCell& rhs)
{
	if (rhs.getValue() == 0)
	{
		throw invalid_argument{ "Divide by zero." };				// 0���� �������ϸ� �ͼ����� �������� �ۼ�
	}
	return SpreadsheetCell { lhs.getValue() / rhs.getValue() };
}

class EvenSequence8
{
public:
	EvenSequence8(std::initializer_list<double> args) 
	{
		if (args.size() % 2 != 0)
		{
			throw std::invalid_argument { "initializer_list should "
				"contain even number of elements." };
		}
		m_sequence.reserve(args.size());
		for (const auto& value : args)
		{
			m_sequence.push_back(value);
		}
	}

	void dump() const
	{
		for (const auto& value : m_sequence)
		{
			printf("%lf, ", value);
		}
		printf("\n");
	}

private:
	std::vector<double> m_sequence;
};

int main()
{
	SpreadsheetCell myCell1{ 4 };
	SpreadsheetCell myCell2{ myCell1 };				// ����
	
	// ����Ʈ �����͸� ������� ���� ��������� ��ü�� ��������� delete�� ȣ���ؼ� �޸𸮸� �����ؾ��Ѵ�.
	SpreadsheetCell* cellPtr1{ new SpreadsheetCell {5} };
	printf("cellPtr1: %lf\n", cellPtr1->getValue());
	delete cellPtr1;
	cellPtr1 = nullptr;			// ���� �� �� ����

	// ��ü�� ����
	SpreadsheetCell myCell3{ 5 }, anotherCell;
	anotherCell = myCell3;


	return 0;
}