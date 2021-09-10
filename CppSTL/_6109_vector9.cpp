#include <iostream>
#include <vector>
using namespace std;

int main6109() {
	vector<int> v(5);

	v[0] = 10;
	v[1] = 20;
	v[2] = 30;
	v[3] = 40;
	v[4] = 50;
	for (unsigned int i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;

	v.clear();			// 모든 원소와 size를 지움
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;

	if (v.empty())
	{
		cout << "v에 원소가 없음" << endl;
	}

	vector<int>().swap(v);
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;

	return 0;
}