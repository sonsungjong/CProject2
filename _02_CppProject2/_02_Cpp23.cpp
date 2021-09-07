#include <iostream>
#include <malloc.h>

using namespace std;

class SSJ_String2
{
private:
	int m_len;			// len
	char* mp_str;	// p_dest_str

public:
	SSJ_String2()		// ��ü ������ (Ŭ������ �̸��� ���ƾ���, ��üȭ�Ҷ� �ڵ����� ȣ��Ǵ� �Լ�)
	{
		init();
	}

	~SSJ_String2()	// ��ü �ı��� (�����ھտ� not������, ��ü�� �������� �ڵ����� ȣ��Ǵ� �Լ�)
	{
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

struct Student
{
	char student[16];
	char name[16];
	int grade;
	char major[64];
};

int main23() {
	struct Student* s = (Student*)malloc(sizeof(struct Student));
	strcpy_s(s->student, "20202020");
	strcpy_s(s->name, "�ռ���");
	s->grade = 4;
	strcpy_s(s->major, "nano");

	struct Student s2;
	strcpy_s(s2.student, "�ռ���");
	s2.grade = 2;

	const char* p_src_str = "sonsungjong";
	// ���� ��ü����
	SSJ_String2 str;		// ��üȭ �����̴ϱ� ������, �ı��ڰ� ȣ���
	// �����޸� �Ҵ�
	// SSJ_String2* p_str = (SSJ_String2*)malloc(sizeof(SSJ_String2));			// malloc�� �����ڿ� �ı��ڸ� ȣ������ ����  --> C++���� malloc�� ����(������)���� ����
	SSJ_String2* p_str = new SSJ_String2;			// new == malloc�����޸��Ҵ� + ������ȣ�� + �ı���ȣ�� + ������ȭ

	//	str.init();		// 100%���	--> ������
	
	p_str->SetString("sonsungjong");
	str.SetString(p_src_str);
	str.ShowString();
	//	str.Destroy();		//	100%���	--> �ı���

	free(s);
	//free(p_str);
	delete p_str;		// new�ϸ� �� delete
	return 0;
}