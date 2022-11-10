#include <iostream>
#include <vector>
using namespace std;

int main154()
{
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int>::iterator normal_iter = v.begin() + 3;				// 40
	vector<int>::reverse_iterator reverse_iter(normal_iter);

	cout << "정방향 반복자의 값 : " << *normal_iter << endl;							// 가리키고 있는 것의 값을 반환
	cout << "역방향 반복자의 값 : " << *reverse_iter << endl;							// 가리키고 있는 것보다 앞에 것(그다음)의 값을 반환

	for (vector<int>::iterator iter = v.begin(); iter != normal_iter; ++iter)
	{
		cout << *iter << " ";				// 정방향 출력
	}
	cout << endl;

	for (vector<int>::reverse_iterator riter = reverse_iter; riter != v.rend(); ++riter)
	{
		cout << *riter << " ";
	}
	cout << endl;

	return 0;
}