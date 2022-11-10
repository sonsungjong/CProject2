#include <iostream>
#include <vector>
using namespace std;

int main182()
{
	vector<int> v(5);					// 5칸짜리 벡터생성
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;

	vector<int>().swap(v);				// 임시벡터를 생성하여 v와 교체하여 용량을 비운다
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;

	cout << "=============" << endl;

	// swap(v)
	vector<int> v1;
	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);

	vector<int> v2;
	v2.push_back(100);
	v2.push_back(200);
	v2.push_back(300);

	for (uint64_t i = 0; i < v1.size(); ++i)
	{
		cout << v1[i] << ", " << v2[i] << endl;
	}
	cout << endl;

	v1.swap(v2);				// v1과 v2를 swap 한다. -> 두 벡터의 원소가 서로 교환된다

	for (uint64_t i = 0; i < v1.size(); ++i)
	{
		cout << v1[i] << ", " << v2[i] << endl;
	}
	cout << endl;

	return 0;
}