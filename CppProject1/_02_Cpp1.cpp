#include <iostream>
#include <string>
#include <malloc.h>
// 동적메모리할당 : 효율적으로 메모리 자원을 사용

using namespace std;

class MY_String {
private:
	int m_len;
	char* mp_str;

public:
	// 무조건 호출해야만 하는 함수를 생성자에 넣게됨
	MY_String()		// 생성자
	{
		// 클래스가 객체로 만들어질 때 자동으로 호출되는 함수
		// 사용자(부하)가 실수로 초기화 함수를 사용하지 않을까봐 생성자를 만듦
		init();
	}
	~MY_String()		// 객체 파괴자
	{
		// 객체가 해제될 때 자동으로 실행되는 함수
		// 사용자(부하)가 실수로 해제하지 않을 수 있기 때문에 파괴자를 만듦
		free(mp_str);
	}

	void init()		// 미해제 방파제를 위한 init함수
	{
		// 사용자가 재사용시 해제를 하지 않을까봐 만든 함수
		m_len = 0;	// 초기화
		mp_str = NULL;
	}

	void SetString(const char* ap_str) {
		Destroy();		// 중복할당 방지 (같은 주소 중복할당 방파제)
		m_len = strlen(ap_str) +1;

		mp_str = (char*)malloc(m_len);	// 동적메모리 할당
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
	// 수준 낮은 부하직원을 간편하게 다루기위해 클래스를 만듦
	// 사용자에 따라 중복할당, 이중해제, 미해제 발생할 수 있음
	
	//str.init();		// 무조건 호출해야만함 -> 사용자(부하)의 실수방지
	str.SetString("tipsware");
	str.ShowString();
	//str.Destroy();	// 무조건 호출해야만함 -> 사용자(부하)의 실수를 미리 차단

	return 0;
}

// 잘하는 사람은 클래스를 정의하고 못하는 사람은 만든 함수를 갖다쓰게만하고