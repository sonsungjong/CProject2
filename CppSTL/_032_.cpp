#include <iostream>

// friend

// 1. �Լ� friend
// 2. Ŭ���� friend
// Ŭ���� ����� ���� ���Ѱ� ������� ��� ����� �� �ְԵ�

// �Լ� ������
class A
{
private:
	int x;
	friend void Func();
};

void Func()
{
	// class A�� ��� ����� ���� ���� ���� ����� �� ����
	A a;
	a.x = 0;
}

// Ŭ���� ������
class B
{
private:
	int y;
	friend class C;
};

class C
{
	// class B�� ��� ����� �������� ������� ��� ����� �� ����
	void SetB_y()
	{
		B b;
		b.y = 0;
	}
};
