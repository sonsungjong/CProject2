#include <iostream>
using namespace std;

static int g_data;		// �ش� ���Ͽ����� ����� �� �ִ� ���������� ��

class Ŭ������{
private:
	int num1 =0 , num2 = 0;
	static int m_data;		// �ش� Ŭ���������� ��밡���� ��������
public:
	Ŭ������()
	{
		static int f_data;		// �ش� �Լ������� ��밡���� �������� (�����ȵ�)
		m_data++;		// �ش� ��ü�� �Ҹ������� +1�ؼ� ����, 
	}
	~Ŭ������()
	{

	}
	void add() const			//	const Ŭ������* const this; ������� ���� �ٲ� �� ���Ե� this-> ���Ұ�
	{
		cout << "add" << endl;
	}
	// �տ����� ���ϰ��� const, �ڿ����� this�����͸� const
};

int Ŭ������::m_data = 0;		// �ѹ��� �ʱ�ȭ�ϵ��� Ŭ���� �ۿ��� �ʱ�ȭ�ϰԲ� ����

int main_const��static() {
	// const : �����Ϸ� �������� ����ó��
	const int data = 2;
	int num = 5;

	const int* p = &num;		// ����Ű�� ����� ���� const (�������� const)
	int* const a = &num;		// a�ּҰ��� const

	// const�� �ٷ� �ڿ� ���� �ָ� �ٲ��� ���ϰ� ó��
	const int* const pp = &num;

	cout << num << endl;

	return 0;
}