#include <iostream>
#include <stdlib.h>
// ������ �����ε�
using namespace std;
// �Լ������� ����� ����� �ܿ��� ����ϰ��ϴϱ� ���ŷο��� �����ڱ�ȣ�� ���
// std.setString("gg"); --> str = "gg";
class Overloadd {
private:
	int m_len;
	char* mp_str;

public:
	// operator��� Ű���带 ����ϸ� �ڿ����� �����ڸ� �Լ������� �ν�
	void operator=(const char* ap_str)
	{
		m_len = strlen(ap_str) + 1;

		mp_str = (char*)malloc(m_len);
		if (mp_str != NULL) {
			memcpy(mp_str, ap_str, m_len);
		}
	}

	int ToInteger() {
		// stdlib.h �� ����
		return atoi(mp_str);
	}

	double ToDouble() {
		return atof(mp_str);
	}

	// ����ȯ ������ �����ε�
	operator int()
	{
		// �����Ϸ� �ڵ�����ȯ ó��
		return atoi(mp_str);
	}

	operator double()
	{
		// ������ �����ε����� �ڵ�����ȯ
		return atof(mp_str);
	}
};

int a;

int main�����ڿ����ε�()
{
	// ������ �����ε� : �Լ����� �����ڷ� ���
	Overloadd str;
	int a;
	
	//str.operator=("tips");	�̷��� ��ȯ����
	str = "tipsoft";		// �Լ����� ���ϰ� �����ڷ� ����

	str = "100";
	int num;
	num = str.ToInteger();		// �Լ�����
	num = str;							// ����ȯ �����ڿ����ε�
	
	cout << "str" << endl;
	cout << num << endl;

	::a = 5;		// ���������� ����
	a = 5;		// ���������� ����
	// :: ������������ ~�ӿ��ִ�
	

	return 0;
}