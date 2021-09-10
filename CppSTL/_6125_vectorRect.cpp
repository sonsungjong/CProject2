#include <iostream>
#include <vector>
using namespace std;

int mainRect() {
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int> v2(v.begin(), v.end());
	vector<int>::iterator iter;
	for (iter = v2.begin(); iter != v2.end(); ++iter) {
		cout << *iter << " ";
	}
	cout << endl;

	vector<int> v3;
	v3.assign(v.begin(), v.end());
	for (iter = v3.begin(); iter != v3.end(); ++iter) {
		cout << *iter << " ";
	}
	cout << endl;

	vector<int> v4 = {23, 45};
	for (iter = v4.begin(); iter != v4.end(); ++iter) {
		cout << *iter << " ";
	}
	cout << endl;

	// 직사각형의 한 좌표 구하기
	vector<vector<int>> v5 = { {10, 10}, {200, 10}, {10,200}};
	vector<int>::iterator pos;
	int x = 0, y =0;

	if (v5[0][0] == v5[1][0]) {
		x = v5[2][0];
	}
	else if (v5[0][0] == v5[2][0]) {
		x = v5[1][0];
	}
	else 
	{
		x = v5[0][0];
	}

	if (v5[0][1] == v5[1][1]) {
		y = v5[2][1];
	}
	else if (v5[0][1] == v5[2][1]) {
		y = v5[1][1];
	}
	else
	{
		y = v5[0][1];
	}
	v5.push_back({ x,y });

	for (pos = v5[3].begin(); pos != v5[3].end(); ++pos) {
		cout << *pos;
		if (!(pos == v5[3].end()-1)) cout << ",";
	}

	return 0;
}