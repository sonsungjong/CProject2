#include <iostream>

using namespace std;

int g_a;

class CMyData
{
private:
	// �ܺο��� �ش� ������ �������� (�Լ��� ����)
	int m_a;
	int m_b;

public:
	// �ܺο��� ����� �� ����
	int member;

	int Sum()		// CMyData *this
	{
		int a;
		int b;

		return m_a + m_b;
	}

	void SetA(int a)
	{
		this->m_a = a;
	}
	void SetB(int b)
	{
		m_b = b;
	}
};

int main_���ٱ���������()
{
	//CMyData data = { 5,-2 };
	CMyData data;		// private�� ��������� �ٸ� ������ ������ �� ����
	int result;

	data.member = 1;
	//data.m_a = 7;
	//data.m_b = 8;
	data.SetA(7);		// ĸ��ȭ: ��ü�� �����ڷḦ �ܺο��� �������� (���ȼ�?, ��������?) --> �Լ������� ����
	data.SetB(8);		// ���������ڸ� ���ؼ� �Լ������� ������... ��ü���ⰳ�� ������

	result = data.Sum();
	cout << result << endl;

	// private ������ ����
	int* p = (int*)&data;
	*p = 99;
	*(p + 1) = 100;
	result = data.Sum();
	cout << result << endl;

	return 0;
}

/*
	C�� ����ü�� �Լ��� ���� �� ����
	C++�� ����ü�� �Լ��� ������ �� �ֱ� ������
	C�� ����ü�� C++�� ����ü�� ȣȯ�� �ȵǴ� ������ �߻�.
	�׷���, �Լ��� �����ϴ� ����ü�� Ŭ������� �������� �̸��� �ٲ�
	struct --> class

	--����������--
	struct�� �⺻���� public
	class�� �⺻���� private

	ĸ��ȭ: ��ü�� �����ڷḦ �ܺο��� �������� (���ȼ�?, ��������?) --> �Լ����� ����
	���������ڸ� ���ؼ� �Լ������� ������... ��ü���ⰳ�� ��Ÿ�� ������
	�Լ��� �ܺκ�ȭ�� ��ó�� �� �ֱ� ������ public

	C : �Լ��� �����͸� ����ϴ� ���, ��ȭ�� �����
	C++ : C�� ��ü������ �߰�, �������̽��� ���� ó���ӵ��� ������

	C: �������� - �ٸ���Ÿ
	C++: ��ü���� (ǥ��ȭ) - ���Ǳ�
*/