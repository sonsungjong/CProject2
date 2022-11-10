#include <iostream>
#include <vector>
using namespace std;

int main191()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int>::iterator iter = v.begin();				// 시작원소를 가리킨다
	cout << *iter << endl;				// 역참조

	iter += 2;				// 다음칸으로 2칸 이동
	cout << *iter << endl;

	iter -= 1;				// 이전칸으로 1칸 이동
	cout << *iter << endl;

	return 0;
}