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
	list<Ÿ��> ������{...ä�ﰪ...};
	for(int val : ������){val} �� ���
*/