#include <iostream>
#include <malloc.h>

using namespace std;

class SSJ_String2
{
private:
	int m_len;			// len
	char* mp_str;	// p_dest_str

public:
	SSJ_String2()		// 객체 생성자 (클래스와 이름이 같아야함, 객체화할때 자동으로 호출되는 함수)
	{
		init();
	}

	~SSJ_String2()	// 객체 파괴자 (생성자앞에 not연산자, 객체가 없어질때 자동으로 호출되는 함수)
	{
		Destroy();
	}

	void init()		// 초기화함수 추가
	{
		m_len = 0;
		mp_str = NULL;
	}
	void SetString(const char* ap_str)
	{
		if (mp_str != NULL)free(mp_str);		// 안전용코드
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
	strcpy_s(s->name, "손성종");
	s->grade = 4;
	strcpy_s(s->major, "nano");

	struct Student s2;
	strcpy_s(s2.student, "손성손");
	s2.grade = 2;

	const char* p_src_str = "sonsungjong";
	// 지역 객체변수
	SSJ_String2 str;		// 객체화 문법이니까 생성자, 파괴자가 호출됨
	// 동적메모리 할당
	// SSJ_String2* p_str = (SSJ_String2*)malloc(sizeof(SSJ_String2));			// malloc은 생성자와 파괴자를 호출하지 않음  --> C++에서 malloc을 문법(연산자)으로 만듦
	SSJ_String2* p_str = new SSJ_String2;			// new == malloc동적메모리할당 + 생성자호출 + 파괴자호출 + 연산자화

	//	str.init();		// 100%사용	--> 생성자
	
	p_str->SetString("sonsungjong");
	str.SetString(p_src_str);
	str.ShowString();
	//	str.Destroy();		//	100%사용	--> 파괴자

	free(s);
	//free(p_str);
	delete p_str;		// new하면 꼭 delete
	return 0;
}