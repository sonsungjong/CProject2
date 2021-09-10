#include <iostream>
#include <vector>
using namespace std;

int main6105() {
	vector<int> v(4);		// size가 5이고 초기값이 0인 벡터를 생성
	
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;
	v.reserve(8);			// 한도 용량을 8로 지정 

	v[0] = 10;
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;
	v[1] = 20;
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;
	v[2] = 30;
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;
	v[3] = 40;
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;
	v.push_back(50);
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;
	v.push_back(60);
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;
	v.push_back(70);
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;
	v.push_back(80);
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;
	v.push_back(90);
	cout << "size: " << v.size() << "\tcapacity: " << v.capacity() << endl;
	
	for (unsigned int i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;

	return 0;
}