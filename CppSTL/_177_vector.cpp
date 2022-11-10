#include <iostream>
#include <vector>
using namespace std;

int main179()
{
	vector<int> v1(5);						// 0으로 초기화된 5칸짜리 컨테이너
	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);
	v1.push_back(40);
	v1.push_back(50);

	for (vector<int>::size_type i = 0; i < v1.size(); ++i)
	{
		cout << v1[i] << " ";												// 0 0 0 0 0 10 20 30 40 50
	}
	cout << endl;

	vector<int> v2(5);						// 0으로 초기화된 5칸짜리 컨테이너
	v2[0] = 10;
	v2[1] = 20;
	v2[2] = 30;
	v2[3] = 40;
	v2[4] = 50;

	for (vector<int>::size_type i = 0; i < v2.size(); ++i)
	{
		cout << v2[i] << " ";												// 10 20 30 40 50
	}
	cout << endl;

	vector<int> v3(5, 10);						// 10으로 초기화된 5칸짜리 컨테이너
	for (vector<int>::size_type i = 0; i < v3.size(); ++i)
	{
		cout << v3[i] << " ";
	}
	cout << endl;
	cout << "size: " << v3.size() << "    capacity: " << v3.capacity() << endl;

	v3.resize(3);						// 3칸짜리 컨테이너로 크기변경
	for (vector<int>::size_type i = 0; i < v3.size(); ++i)
	{
		cout << v3[i] << " ";
	}
	cout << endl;
	cout << "size: " << v3.size() << "    capacity: " << v3.capacity() << endl;

	v3.resize(10, 33);							// 10칸짜리로 확장하면서 추가 생성되는 칸은 초기값을 33
	for (vector<int>::size_type i = 0; i < v3.size(); ++i)
	{
		cout << v3[i] << " ";
	}
	cout << endl;
	cout << "size: " << v3.size() << "    capacity: " << v3.capacity() << endl;

	return 0;
}
