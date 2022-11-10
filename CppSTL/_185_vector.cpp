#include <iostream>
#include <vector>
using namespace std;

// at()
int main185()
{
	// v[i] : 범위를 체크하지 않아 빠름
	// v.at(i) : 느리지만 범위를 체크해 안전함
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for (vector<int>::size_type i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	v[0] = 100;				// 범위 점검 없이 0 index 참조
	v[4] = 500;				// 범위 점검 없이 4 index 참조
	for (vector<int>::size_type i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	v.at(0) = 1000;				// 범위 점검 후에 0 index 참조
	v.at(4) = 5000;				// 범위 점검 후에 4 index 참조
	for (vector<int>::size_type i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	// 결과는 같고 범위를 체크하냐 안하냐 차이
	cout << "=======" << endl;
	try
	{
		cout << v.at(0) << endl;
		cout << v.at(3) << endl;
		cout << v.at(6) << endl;						// throw : out_of_range 예외
	}
	catch (out_of_range& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}