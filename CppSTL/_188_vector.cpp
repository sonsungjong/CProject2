#include <iostream>
#include <vector>
using namespace std;

int main188()
{
	// assign(n, x)
	vector<int> v(5, 1);					// 5칸짜리 벡터를 1로 초기화
	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	v.assign(5, 2);				// 5개의 원소값을 2로 할당
	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	return 0;
}