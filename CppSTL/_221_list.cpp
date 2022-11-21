#include <iostream>
#include <list>

using namespace std;

int main()
{
	list<int> lst;
	lst.push_back(10);
	lst.push_back(20);
	lst.push_back(30);
	lst.push_back(40);
	lst.push_back(50);

	list<int>::iterator iter = lst.begin();
	list<int>::iterator iter2;
	++iter;
	++iter;

	iter2 = lst.erase(iter);
	for (iter = lst.begin(); iter != lst.end(); ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;

	cout << "iter2 : " << *iter2 << endl;

	iter = iter2;
	iter2 = lst.insert(iter, 300);
	for (iter = lst.begin(); iter != lst.end(); ++iter)
	{
		cout << *iter << ' ';
	}
	cout << endl;

	cout << "iter2 : " << *iter2 << endl;

	return 0;
}