// const ��� �Լ��� �� const ��� �Լ�
#include <iostream>
using namespace std;

class Point2
{
	int x;
	int y;
public:
	Point2(int _x = 0, int _y = 0) : x(_x), y(_y) {}

	// ��� ������ �ٲ��� �����Ƿ� const�Լ�
	int GetX() const
	{
		return x;
	}

	int GetY() const					// const �Լ�
	{
		return y;
	}

	void SetX(int a_x)				// �� const �Լ� : ��� ����
	{
		x = a_x;
	}

	void SetY(int a_y)				// �� const �Լ� : ��� ����
	{
		y = a_y;
	}

	void Print() const
	{
		cout << x << ',' << y << endl;
	}
};

int main020()
{
	const Point2 p1(0, 0);				// const ��ü : ��� ��� ���� �Ұ� (const �Լ��� ȣ�Ⱑ��)
	Point2 p2(2, 3);				// �� const ��ü

	p1.Print();
	p2.Print();

	cout << "p1: " << p1.GetX() << ',' << p1.GetY() << endl;
	cout << "p2: " << p2.GetX() << ',' << p2.GetY() << endl;

	// p1.SetX(5);						// �����Ϸ� ���� -> const ��ü�� const ����Լ��� ��� ����
	// p1.SetY(5);						// �����Ϸ� ���� -> const ��ü�� const ����Լ��� ��� ����
	p2.SetX(5);
	p2.SetY(5);

	return 0;
}
