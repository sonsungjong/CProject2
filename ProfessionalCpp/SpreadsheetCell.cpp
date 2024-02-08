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
	// 오버로딩
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
	//setString(initialValue);				// 위임생성자로 대체 (생성자에서 다른 생성자 호출)
}

SpreadsheetCell& SpreadsheetCell::operator=(const SpreadsheetCell& rhs)
{
	if (this == &rhs) 
	{
		// 자기 자신을 대입할 경우
		return *this;
	}
	else {
		m_value = rhs.m_value;					// 값을 복사
		return *this;
	}
}

//SpreadsheetCell::SpreadsheetCell()
//{
//	m_value = 0;
//}

// 전역함수 (매개변수 위치 자유화)
SpreadsheetCell operator+(const SpreadsheetCell& lhs, const SpreadsheetCell& rhs)
{
	return SpreadsheetCell{ lhs.getValue() + rhs.getValue() };
}

SpreadsheetCell operator/(const SpreadsheetCell& lhs, const SpreadsheetCell& rhs)
{
	if (rhs.getValue() == 0)
	{
		throw invalid_argument{ "Divide by zero." };				// 0으로 나누려하면 익셉션을 던지도록 작성
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
	SpreadsheetCell myCell2{ myCell1 };				// 복제
	
	// 스마트 포인터를 사용하지 않은 프리스토어 객체는 명시적으로 delete를 호출해서 메모리를 해제해야한다.
	SpreadsheetCell* cellPtr1{ new SpreadsheetCell {5} };
	printf("cellPtr1: %lf\n", cellPtr1->getValue());
	delete cellPtr1;
	cellPtr1 = nullptr;			// 해제 후 빈값 대입

	// 객체에 대입
	SpreadsheetCell myCell3{ 5 }, anotherCell;
	anotherCell = myCell3;


	return 0;
}