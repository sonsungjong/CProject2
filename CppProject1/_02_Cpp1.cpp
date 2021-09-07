#include <iostream>
#include <string>
#include <malloc.h>
// �����޸��Ҵ� : ȿ�������� �޸� �ڿ��� ���

using namespace std;

class MY_String {
private:
	int m_len;
	char* mp_str;

public:
	// ������ ȣ���ؾ߸� �ϴ� �Լ��� �����ڿ� �ְԵ�
	MY_String()		// ������
	{
		// Ŭ������ ��ü�� ������� �� �ڵ����� ȣ��Ǵ� �Լ�
		// �����(����)�� �Ǽ��� �ʱ�ȭ �Լ��� ������� ������� �����ڸ� ����
		init();
	}
	~MY_String()		// ��ü �ı���
	{
		// ��ü�� ������ �� �ڵ����� ����Ǵ� �Լ�
		// �����(����)�� �Ǽ��� �������� ���� �� �ֱ� ������ �ı��ڸ� ����
		free(mp_str);
	}

	void init()		// ������ �������� ���� init�Լ�
	{
		// ����ڰ� ����� ������ ���� ������� ���� �Լ�
		m_len = 0;	// �ʱ�ȭ
		mp_str = NULL;
	}

	void SetString(const char* ap_str) {
		Destroy();		// �ߺ��Ҵ� ���� (���� �ּ� �ߺ��Ҵ� ������)
		m_len = strlen(ap_str) +1;

		mp_str = (char*)malloc(m_len);	// �����޸� �Ҵ�
		if (mp_str != NULL) {
			memcpy(mp_str, ap_str, m_len);
		}
	}

	void Destroy() {
		free(mp_str);
		init();
	}

	void ShowString() {
		cout << mp_str << endl;
	}
};

int main2day1()
{
	MY_String str;
	// ���� ���� ���������� �����ϰ� �ٷ������ Ŭ������ ����
	// ����ڿ� ���� �ߺ��Ҵ�, ��������, ������ �߻��� �� ����
	
	//str.init();		// ������ ȣ���ؾ߸��� -> �����(����)�� �Ǽ�����
	str.SetString("tipsware");
	str.ShowString();
	//str.Destroy();	// ������ ȣ���ؾ߸��� -> �����(����)�� �Ǽ��� �̸� ����

	return 0;
}

// ���ϴ� ����� Ŭ������ �����ϰ� ���ϴ� ����� ���� �Լ��� ���پ��Ը��ϰ�