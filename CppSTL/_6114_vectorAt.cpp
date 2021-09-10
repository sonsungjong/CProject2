#include <iostream>
#include <vector>
using namespace std;
int main6114() {
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	v[0] = 1;
	v[1] = 2;
	v[2] = 3;
	v[3] = 4;
	v[4] = 5;
	for (unsigned int i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;

	v.at(0) = 1000;
	v.at(1) = 2000;
	v.at(2) = 3000;
	v.at(3) = 4000;
	v.at(4) = 5000;
	for (unsigned int i = 0; i < v.size(); ++i) {
		cout << v.at(i) << " ";
	}
	cout << endl;

	v.assign(5, 60);
	for (vector<int>::iterator iter = v.begin(); iter != v.end(); ++iter) {				// ++iter를 하면 다음 원소를 가리키게됨
		cout << *iter << " ";
	}

	return 0;
}