#include <iostream>
using namespace std;
void Client();

// 서버
void PrintHello() {
	cout << "Hello, Server Start!" << endl;
	Client();			// 콜백 (서버에서 클라이언트 호출)
}

// 클라이언트
void Client()
{
	cout << "I am Client" << endl;
}

int main236()
{
	PrintHello();			// 서버 호출

	return 0;
}

/*
	클라이언트가 서버 호출 : 콜 (Call)
	서버가 클라이언트 호출 : 콜백 (CallBack)
*/