#include <iostream>
#include <vector>
using namespace std;

int main197()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int>::iterator iter = v.begin() + 2;
	vector<int>::iterator iter2;

	// iter가 가리키는 위치에 정수 100 삽입
	// iter2는 삽입한 정수를 가리키는 반복자
	iter2 = v.insert(iter, 100);				// 가리키는 위치에 넣고 있던 값은 뒤로 밀어버림

	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << endl;
	}
	cout << endl;

	cout << "iter2: " << *iter2 << endl;

	return 0;
}