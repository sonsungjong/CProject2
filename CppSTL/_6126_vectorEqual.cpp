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
		cout << "v1과 v2는 모두 같다" << endl;
	}
	if (v1 != v2)
	{
		cout << "v1과 v2는 하나라도 다르다" << endl;
	}
	if (v1 < v2)
	{
		cout << "문자열비교 했을 때 v1 < v2" << endl;
		// 10, 20, 30, 40, 50 and 10, 20, 50  ==> 30 vs 50
	}
	
	return 0;
}