#include <iostream>
// ������ �����ε�

using namespace std;

void showString();

class Cpp32 
{
private:
	char* mp_str;
	int m_len;
	int m_max_len;
public:
	Cpp32(){
		init();
	}
	~Cpp32(){
		destroy();
	}
	void setString(const char* ap_str) {
		m_len = strlen(ap_str) + 1;
		if (m_max_len < m_len)
		{
			free(mp_str);
			m_max_len = m_len;
			mp_str = (char*)malloc(m_max_len);
		}
		memcpy(mp_str, ap_str, m_len);
	}

	
	void init() {
		m_len = 0;
		mp_str = NULL;
	}
	void destroy()
	{
		free(mp_str);
		init();
	}
	void showString();
	int toInt()
	{
		return atoi(mp_str);		// "100" -> 100
	}
	// ����ȯ������ �����ε��� �ڷ���X
	operator int() {
		return atoi(mp_str);
	}		//  ������� �ʾƵ� �˾Ƽ� �����Ϸ��� �����
	double toDouble()
	{
		return atof(mp_str);		// "100.5" -> 100.5
	}
	operator double() {
		::showString();		// ���� ������ �ִ� showString() �Լ��� ���
		return atof(mp_str);
	}
	// ���Կ����� �����ε�
	void operator=(const char* ap_str)
	{
		m_len = strlen(ap_str) + 1;
		if (m_max_len < m_len)
		{
			free(mp_str);
			m_max_len = m_len;
			mp_str = (char*)malloc(m_max_len);
		}
		memcpy(mp_str, ap_str, m_len);
	}
};

int a;
void showString() {
	cout << "�ڵ� ����ȯ" << endl;
}

// �������� �ٸ� Ŭ�������� �Լ� ����, �и��ϸ� �ҽ����� ������ �ʿ���� (����� �ҽ� �и�)
void Cpp32::showString()
{
	cout << mp_str << endl;
}

int main32()
{
	int a;
	Cpp32 str;
	// ���Կ����� �����ε�
	str = "100000";
	str.showString();
	str = "�ȳ��ϼ���!";
	str.showString();
	str.setString("�ȳ��ϼ���");
	str.showString();
	str = "30";
	str.showString();

	//int num = str.toInt();
	int num1 = str;		// int����ȯ������ �����ε��� �ڵ����� ����
	double num2 = str;		// double����ȯ������ �����ε��� �ڵ����� ���

	::a = 5;		// ��������
	a = 5;		// ��������

	return 0;
}

/*
	������ �����ε� : �Լ��̸��� �����ڷ� ���, ���⿡ ���� ������
	str.setString("") --> str = ""
	�̸������� �����, �̸��� �ܿ����ϴ� ���ŷο� �ؼ�
	����� ���忡�� ���ϰ� ���
*/