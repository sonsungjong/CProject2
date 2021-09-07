#include <iostream>
// 상속 : 컴파일을 통해 클래스를 복제하는 기능

using namespace std;

class MyClass 
{
private:
	string str;
public:
	void setString(string str) {
		this->str = str;
	}
	string getString() {
		return this->str;
	}
};

class MyClassEx : public MyClass
{
	// 컴파일러가 MyClass를 복사-붙여넣기 해줌
	// 상속 : 어떤 클래스를 복사 후 붙여넣기하여 함수를 변경하거나 추가할 때 사용

};


int main41() {
	MyClassEx strEx;
	strEx.setString("안녕하세요 반갑습니다");
	cout << strEx.getString() << endl;
	return 0;
}

/*
	상속
	C언어를 하다보면 기존의 함수를 유지하면서 조금 바뀐 함수를 하나더 요구한다.
	Add(), AddEx()
	그렇게되면 함수를 복사, 붙여넣기 하는 경우가 많음
	-> 비슷한 함수가 엄청 많아짐
	중복작업이 많아져서 유지보수가 불리해짐 (유사한 함수를 각자 개별적으로 처리)

	Cpp는 클래스단위로 중복이 일어남
	다른 사람이 쓰고 있기 때문에 기존 클래스를 유지하면서 기능을 추가하고자한다면
	클래스를 복사, 붙여넣기해서 새로 만들어 클래스 이름을 바꾸고 코드를 추가하는 경우가 많다.
	
	중복코드를 어떻게 피할 수 있을까
	-> 개발자가 복사, 붙여넣기하게하지 않고
	컴파일러가 기계어로 번역할때 복사, 붙여넣기하도록 만들음 : 상속

	중복 편집의 문제가 해결됨

	상속 : 복사해서 붙여넣기
	
	void main(){
		MyClassEx mc;
		mc.부모의함수();
	}
	상속(copy)을 하여 원하는 기능 추가
	오버라이딩을 통해 변경하고자하는 함수 재정의

	protected : 상속받은 자식은 사용이 가능한 private
	
*/