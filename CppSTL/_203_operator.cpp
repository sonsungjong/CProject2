#include <iostream>
#include <vector>
using namespace std;

int main203()
{
	vector<int> v1;
	vector<int> v2;

	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);
	v1.push_back(40);
	v1.push_back(50);

	v2.push_back(10);
	v2.push_back(20);
	v2.push_back(50);

	if (v1 == v2)						// 모든 원소가 같은가
	{
		cout << "v1 == v2" << endl;
	}

	if (v1 != v2)						// 하나라도 다른 원소가 있는가
	{
		cout << "v1 != v2" << endl;
	}

	if (v1 < v2)						// 순차열의 원소를 하나씩 비교하여 v1의 원소가 작다면 참, 아니면 거짓 (문자열처럼 원소를 하나씩 비교한다.)
	{
		cout << "v1 < v2" << endl;
	}

	return 0;
}