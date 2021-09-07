#include <iostream>

using namespace std;

class SSJ_String
{
private:
	int m_len;			// len
	char* mp_str;	// p_dest_str

public:
	SSJ_String()		// 객체 생성자 (클래스와 이름이 같아야함) Constructor
	{
		// 객체 변수를 생성할 때 자동으로 호출되는 함수
		// 내 클래스를 사용하는 부하 개발자가 실수로 처음 사용해줘야하는 코드를 작성하지 않을 경우를 대비
		// 100% 먼저 사용해줘야하는 코드를 작성
		init();
	}

	~SSJ_String()	// 객체 파괴자 (생성자앞에 not연산자) Destructor
	{
		// 객체 변수가 없어질 때 자동으로 호출되는 함수
		// 내 클래스를 사용하는 부하 개발자가 실수로 해제하지 않을 경우를 대비
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

int main22() {
	const char* p_src_str = "sonsungjong";
	SSJ_String str;

//	str.init();		// 100%사용	--> 생성자
	// 포인터도 몰라도 되고 메모리할당 및 해제도 몰라도됨
	str.SetString(p_src_str);
	str.ShowString();
//	str.Destroy();		//	100%사용	--> 파괴자

	return 0;
}

/*
	C++이 추구하는 코드의 전개방식 (class화)
	생성자와 파괴자를 만들어서 클래스를 만드는 사람이 편해짐
	사용자의 실수를 방지
*/