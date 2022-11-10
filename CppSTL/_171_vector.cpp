#include <iostream>
#include <vector>
using namespace std;

int main171()
{
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

	// size() : 원소의 갯수
	cout << v.size() << endl;

	// capacity() : 할당된 메모리 크기
	cout << v.capacity() << endl;									// vector는 효율을 위해 추가 전에 미리 공간을 확보해놓음

	// max_size() : 최대 저장 가능한 원소의 갯수
	cout << v.max_size() << endl;

	cout << "=============" << endl;

	// capacity 메모리 재할당
	vector<int> v2;

	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(10);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(20);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(30);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(40);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(50);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(60);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(70);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(80);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(90);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	
	for (unsigned __int64 i = 0; i < v2.size(); ++i)
	{
		cout << v2[i] << " ";
	}
	cout << endl;

	return 0;
}
