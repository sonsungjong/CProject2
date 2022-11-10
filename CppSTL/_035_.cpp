#include <iostream>
using namespace std;

struct FuncObject
{
public:
	void operator() (int arg) const
	{
		cout << "정수 : " << arg << endl;
	}
	void operator() (int arg1, int arg2) const
	{
		cout << "정수 : " << arg1 << ',' << arg2 << endl;
	}
	void operator()(int arg1, int arg2, int arg3) const
	{
		cout << "정수 : " << arg1 << ',' << arg2 << ',' << arg3 << endl;
	}
};

int main035()
{
	FuncObject print;
	print(10);						// 객체 생성 후 호출(암시적)
	print(10, 20);
	print(10, 20, 30);
	cout << endl;

	print.operator()(10);			// 객체 생성 후 호출 (명시적)
	print.operator()(10, 20);
	print.operator()(10, 20, 30);
	cout << endl;

	// 임시객체 : 클래스 이름으로 임시객체를 생성
	// 그 문장에서 생성되고 그 문장을 벗어나면 소멸됨, 해당 문장에서만 임시로 필요한 객체에 사용
	FuncObject()(10);					// 임시 객체로 호출(암시적)
	FuncObject()(10, 20);
	FuncObject()(10, 20, 30);
	cout << endl;

	FuncObject().operator()(10);				// 임시 객체로 호출 (명시적)
	FuncObject().operator()(10, 20);
	FuncObject().operator()(10, 20, 30);

	return 0;
}