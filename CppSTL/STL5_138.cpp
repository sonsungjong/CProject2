// 반복자 : 컨테이너에 저장된 원소를 순회하고 접근하는 방법을 제공함 (인터페이스)

// 컨테이너의 원소를 가리키고 접근함
// 다음 원소로 이동하고 모든 원소를 순회할 수 있음
// begin, end, *, ++, !=, ==
//  [begin, end)

#include <iostream>
#include <vector>
using namespace std;

int main()
{
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int>::iterator iter;						// 반복자 생성 (아직 원소를 가리키지 않음)
	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << endl;						// 역참조로 조회
	}

	return 0;
}