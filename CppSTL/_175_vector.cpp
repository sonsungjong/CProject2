#include <iostream>
#include <vector>
using namespace std;

int main175()
{
	// reserve

	vector<int> v;
	v.reserve(8);					// 8칸의 메모리를 미리 할당
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;
	v.push_back(10);
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;
	v.push_back(20);
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;
	v.push_back(30);
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;
	v.push_back(40);
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;
	v.push_back(50);
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;
	v.push_back(60);
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;
	v.push_back(70);
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;
	v.push_back(80);
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;
	v.push_back(90);
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;
	
	for (unsigned __int64 i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	return 0;
}