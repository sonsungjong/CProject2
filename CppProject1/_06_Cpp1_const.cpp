#include <iostream>
using namespace std;

static int g_data;		// 해당 파일에서만 사용할 수 있는 전역변수가 됨

class 클래스명{
private:
	int num1 =0 , num2 = 0;
	static int m_data;		// 해당 클래스에서만 사용가능한 전역변수
public:
	클래스명()
	{
		static int f_data;		// 해당 함수에서만 사용가능한 전역변수 (해제안됨)
		m_data++;		// 해당 객체가 불릴때마다 +1해서 저장, 
	}
	~클래스명()
	{

	}
	void add() const			//	const 클래스명* const this; 멤버변수 값을 바꿀 수 없게됨 this-> 사용불가
	{
		cout << "add" << endl;
	}
	// 앞에오면 리턴값을 const, 뒤에오면 this포인터를 const
};

int 클래스명::m_data = 0;		// 한번만 초기화하도록 클래스 밖에서 초기화하게끔 구성

int main_const와static() {
	// const : 컴파일러 차원에서 에러처리
	const int data = 2;
	int num = 5;

	const int* p = &num;		// 가리키는 대상의 값을 const (역참조값 const)
	int* const a = &num;		// a주소값을 const

	// const는 바로 뒤에 오는 애를 바꾸지 못하게 처리
	const int* const pp = &num;

	cout << num << endl;

	return 0;
}