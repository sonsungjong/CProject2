#include <iostream>
using namespace std;

// �迭 �ε��� ������ �����ε� [] : ���� ��ü�� �����ϰ� �����ϴ� ��ü�� ��� (�����̳� ��ü)

class Point
{
	int x;
	int y;
public:
	Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const { cout << x << ',' << y << endl; }

	int operator[] (int idx) const
	{
		if (idx == 0) {
			return x;
		}
		else if (idx == 1) {
			return y;
		}
		else {
			throw "�Ұ���";
		}
	}
};

int main037()
{
	Point pt(1, 2);
	pt.Print();
	cout << pt[0] << ',' << pt[1] << endl;				// pt.operator[](0)
	// pt.operator[](1)

	return 0;
}