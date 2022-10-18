#include <iostream>
using namespace std;

// 클라이언트가 서버를 사용하면 콜(call)
// 서버가 클라이언트를 사용하면 콜백(callback)

void Client();				// 사실상 클라이언트의 내용을 미리 아는 것은 불가능

// 서버
void PrintHello()
{
	cout << "Hello!" << endl;
	Client();					// 서버에서 클라이언트의 코드를 호출
}

// 클라이언트
void Client()
{
	cout << "난 client" << endl;
}

int main067()
{
	PrintHello();					// 서버 코드 호출

	return 0;
}