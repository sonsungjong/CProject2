#include <iostream>

using namespace std;

class TT_String
{
protected:
	int m_len, m_max_len;
	char* mp_str;
public:
	virtual void SetString(const char* ap_str) {
		m_len = strlen(ap_str) + 1;
		if (m_max_len < m_len) {
			free(mp_str);

			m_max_len = m_len;
			mp_str = (char*)malloc(m_max_len);
		}
		memcpy(mp_str, ap_str, m_len);
	}
};

class TT_StringEx : public TT_String
{
public:
	void SetString(const char* ap_str)
	{
		TT_String::SetString(ap_str);
		cout << mp_str << endl;
	}
};

int main51() {
	TT_StringEx str;		// �ڽ� ��ü
	TT_String* p;		// �θ� ������
	p = &str;
	// p->TT_StringEx::SetString("taptap");
	// p->TT_String::SetString("taptap");
	p->SetString("taptap");		// virtual �ϸ� �ڽ����� ���

	// �θ� �Լ��� virtual (����������, �޸𸮸���)
	// 	�ڽ��Լ��� ȣ���Ϸ��ߴµ� �����͸� �θ���� ����ϴٺ��ϱ� �θ��Լ��� ȣ���ϰ� �� ���
	// �ڽĿ��� �ٲ� ���ɼ��� ���� �Լ��� �θ��Լ��� virtual

	// virtual : ������ �������̵��� �����Ϸ��� ������ �� �ִ� �κ��� ó��
	return 0;
}

// 38��