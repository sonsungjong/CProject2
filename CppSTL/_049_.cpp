#include <iostream>
using namespace std;

// explicit : �������� ȣ���� ����� ȣ�⸸ �����ϰ� ����� �ǵ�ġ���� ������ ����ȯ�� ���� (�����ڰ� ���ڸ� ���� �� ������ �տ��ٰ� ���)

class Point
{
	int x;
	int y;
public:
	// ������ �̸� �տ� explicit�� �ٿ��ָ� ����� ������ȣ�⸸ ������
	// ���ڸ� ���� ������ �տ��� explicit�� �ٿ��־� �Ͻ��� ������ ����ȯ�� �����ִ� ���� ���� (����� ȣ�⸸ ��������)
	explicit Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const { cout << x << ',' << y << endl; }
};

int main049()
{
	Point pt;
	pt.Print();

	// �Ͻ��� ������ ȣ�� (explicit �� �ٿ��ָ� �Ұ���)
	// pt = 10;				// Point(10, 0);

	pt.Print();

	return 0;
}
