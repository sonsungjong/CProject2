#include <iostream>
using namespace std;

bool Pred_less(int a, int b)
{
	return a < b;
}

struct Less
{
	bool operator()(int a, int b)
	{
		return a < b;
	}
};

struct Greater
{
	bool operator()(int a, int b)
	{
		return a > b;
	}
};

int main82()
{
	Less less;

	// 전역함수 호출
	cout << Pred_less(10, 20) << endl;
	cout << Pred_less(20, 10) << endl;
	cout << endl;

	// 객체로 함수객체 호출
	cout << less(10, 20) << endl;
	cout << less(20, 10) << endl;

	// 임시객체로 함수객체 호출
	cout << Less()(10, 20) << endl;
	cout << Less()(20, 10) << endl;

	cout << endl;
	cout << less.operator()(10, 20) << endl;	
	cout << Less().operator()(10, 20) << endl;

	cout << "=====================" << endl;
	cout << Greater()(10, 20) << endl;
	cout << Greater()(20, 10) << endl;
	cout << endl;

	return 0;
}