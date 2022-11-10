#include <iostream>
using namespace std;

class MyCmp
{
private:
	int m_result;
public:
	int operator()(int a, int b)
	{
		if (a == b)
		{
			m_result = 1;
		}
		else 
		{
			m_result = 0;
		}
		return m_result;
	}
};

class Adder2
{
private:
	int m_total;
public:
	int operator()(int a, int b)
	{
		m_total = a + b;
		return m_total;
	}
};

int main88()
{
	MyCmp cmp;
	Adder2 add;

	if (cmp(10, 20))
	{
		cout << "����" << endl;
	}
	else
	{
		cout << "�ٸ���" << endl;
	}

	int sum = add(10, 20);
	cout << "sum= " << sum << endl;

	return 0;
}