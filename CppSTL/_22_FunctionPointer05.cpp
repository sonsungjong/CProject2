#include <iostream>

using namespace std;

class Vertices
{
protected:
	int m_x;
	int m_y;
public:
	explicit Vertices(int aX = 0, int aY = 0) : m_x(aX), m_y(aY) {}
	void Print() {
		cout << m_x << "," << m_y << endl;
	}
	void PrintInt(int n) {
		cout << "�׽�Ʈ ���� : " << n << endl;
	}
};

int main225()
{
	Vertices vertices(2, 3);
	Vertices* p_vertices = &vertices;

	void (Vertices:: * pf1)();
	pf1 = &Vertices::Print;

	void (Vertices:: * pf2)(int);
	pf2 = &Vertices::PrintInt;

	(vertices.*pf1)();				// ��ü�� ����Լ������͸� �̿��� ȣ��
	(vertices.*pf2)(10);
	cout << endl;
	(p_vertices->*pf1)();			// �ּҷ� ����Լ������͸� �̿��� ȣ��
	(p_vertices->*pf2)(10);

	return 0;
}