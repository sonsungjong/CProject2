#include <iostream>
#include <vector>
using namespace std;

int main180()
{
	vector<int> v(5);				// 5칸짜리 미리 생성

	v[0] = 10;
	v[1] = 20;
	v[2] = 30;
	v[3] = 40;
	v[4] = 50;
	for (uint64_t i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;

	v.clear();				// 벡터를 비운다
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;

	if (v.empty())						// 비어있는지 확인한다
	{
		cout << "v에 원소가 없습니다." << endl;
	}

	return 0;
}