#include <iostream>

using namespace std;

class SSJ_String
{
private:
	int m_len;			// len
	char* mp_str;	// p_dest_str

public:
	SSJ_String()		// ��ü ������ (Ŭ������ �̸��� ���ƾ���) Constructor
	{
		// ��ü ������ ������ �� �ڵ����� ȣ��Ǵ� �Լ�
		// �� Ŭ������ ����ϴ� ���� �����ڰ� �Ǽ��� ó�� ���������ϴ� �ڵ带 �ۼ����� ���� ��츦 ���
		// 100% ���� ���������ϴ� �ڵ带 �ۼ�
		init();
	}

	~SSJ_String()	// ��ü �ı��� (�����ھտ� not������) Destructor
	{
		// ��ü ������ ������ �� �ڵ����� ȣ��Ǵ� �Լ�
		// �� Ŭ������ ����ϴ� ���� �����ڰ� �Ǽ��� �������� ���� ��츦 ���
		Destroy();
	}

	void init()		// �ʱ�ȭ�Լ� �߰�
	{
		m_len = 0;
		mp_str = NULL;
	}
	void SetString(const char* ap_str)
	{
		if (mp_str != NULL)free(mp_str);		// �������ڵ�
		mp_str = NULL;

		m_len = strlen(ap_str);
		
		mp_str = (char*)malloc(m_len + 1);
		strcpy_s(mp_str, m_len + 1, ap_str);
	}

	void Destroy()
	{
		if (mp_str != NULL) {
			free(mp_str);
			mp_str = NULL;
		}
	}

	void ShowString() 
	{
		cout << mp_str << endl;
	}
};

int main22() {
	const char* p_src_str = "sonsungjong";
	SSJ_String str;

//	str.init();		// 100%���	--> ������
	// �����͵� ���� �ǰ� �޸��Ҵ� �� ������ ���󵵵�
	str.SetString(p_src_str);
	str.ShowString();
//	str.Destroy();		//	100%���	--> �ı���

	return 0;
}

/*
	C++�� �߱��ϴ� �ڵ��� ������� (classȭ)
	�����ڿ� �ı��ڸ� ���� Ŭ������ ����� ����� ������
	������� �Ǽ��� ����
*/