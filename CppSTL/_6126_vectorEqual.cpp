#include <iostream>
#include <vector>
using namespace std;

int main6126() {
	vector<int> v1, v2;

	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);
	v1.push_back(40);
	v1.push_back(50);

	v2.push_back(10);
	v2.push_back(20);
	v2.push_back(50);

	if (v1 == v2) 
	{
		cout << "v1�� v2�� ��� ����" << endl;
	}
	if (v1 != v2)
	{
		cout << "v1�� v2�� �ϳ��� �ٸ���" << endl;
	}
	if (v1 < v2)
	{
		cout << "���ڿ��� ���� �� v1 < v2" << endl;
		// 10, 20, 30, 40, 50 and 10, 20, 50  ==> 30 vs 50
	}
	
	return 0;
}