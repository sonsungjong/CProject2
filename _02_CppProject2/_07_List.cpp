#include <iostream>
#include <list>

using namespace std;

int mainL() {
	list<int> _list{1,2,3};

	for (int _value : _list)
	{
		cout << _value << endl;
	}

	return 0;
}

/*
	-list-
	list<타입> 변수명{...채울값...};
	for(int val : 변수명){val} 로 출력
*/