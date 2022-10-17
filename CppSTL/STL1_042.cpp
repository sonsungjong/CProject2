#include <iostream>
using namespace std;

// �޸�����, Ŭ����������� ������ �����ε� (*, ->) : �ݺ��ڳ� ����Ʈ�������� ��ü�� ����

// �Ϲ�Ŭ����
class Point
{
	int x;
	int y;
public:
	Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const { cout << x << ',' << y << endl; }
};

// ����Ʈ������ (�ڵ� ����)
class PointPtr
{
	Point *ptr;
public:
	PointPtr(Point *p) : ptr(p) {}
	~PointPtr()
	{
		delete ptr;
	}

	// PointŬ������ ����Լ� ����� ���� ->������ �����ε��� �������
	Point* operator->() const
	{
		return ptr;
	}

	Point& operator*() const
	{
		return *ptr;
	}
};

int main042()
{
	Point* p1 = new Point(2, 3);				// �Ϲ�������
	PointPtr p2 = new Point(5, 5);				// ����Ʈ������

	p1->Print();
	p2->Print();				// p2.operator->()->Print();
	cout << endl;

	(*p1).Print();
	(*p2).Print();					// p2.operator*().Print();

	delete p1;
	//delete p2;					// �޸� �ڵ�����

	return 0;
}