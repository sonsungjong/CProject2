#include <iostream>
#include <list>

using namespace std;

int main220()
{
	list<int> lst;

	// 뒤로 추가
	lst.push_back(10);
	lst.push_back(20);
	lst.push_back(30);
	lst.push_back(40);
	lst.push_back(50);

	// list의 양방향 반복자
	list<int>::iterator iter;
	for (iter = lst.begin(); iter != lst.end(); ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;

	// 앞으로 추가
	lst.push_front(100);
	lst.push_front(200);

	for (iter = lst.begin(); iter != lst.end(); ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;

	return 0;
}