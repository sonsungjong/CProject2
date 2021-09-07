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
	TT_StringEx str;		// 자식 객체
	TT_String* p;		// 부모 포인터
	p = &str;
	// p->TT_StringEx::SetString("taptap");
	// p->TT_String::SetString("taptap");
	p->SetString("taptap");		// virtual 하면 자식으로 사용

	// 부모 함수에 virtual (다형성전용, 메모리먹음)
	// 	자식함수를 호출하려했는데 포인터를 부모것을 사용하다보니까 부모함수를 호출하게 될 경우
	// 자식에서 바꿀 가능성이 높은 함수만 부모함수에 virtual

	// virtual : 다형성 오버라이딩시 컴파일러가 오해할 수 있는 부분을 처리
	return 0;
}

// 38분