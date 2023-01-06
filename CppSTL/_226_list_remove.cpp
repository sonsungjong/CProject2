#include <cstdio>
#include <list>

using namespace std;
// remove : 해당 원소를 갖고있는 모든 노드를 제거 (유일하게 반복자가 필요없음, 속도가 빠름)
// remove_if : 함수포인터를 통해서 조건에 해당하는 원소를 모두 제거

bool Predicate(int n) {
	return 10 <= n && n <= 30;
}

int main226()
{
	list<int> lst;

	lst.push_back(10);
	lst.push_back(20);
	lst.push_back(30);
	lst.push_back(10);
	lst.push_back(40);
	lst.push_back(50);
	lst.push_back(10);
	lst.push_back(10);

	list<int>::iterator iter;

	// remove 전
	for (iter = lst.begin(); iter != lst.end(); ++iter)
	{
		printf("%d ", *iter);
	}
	printf("\n");

	// remove 후
	lst.remove(10);				// 10 원소의 노드를 모두 제거
	for (iter = lst.begin(); iter != lst.end(); ++iter) 
	{
		printf("%d ", *iter);
	}
	printf("\n");

	// remove_if
	lst.remove_if(Predicate);					// 조건자가 참인 모든 원소를 제거
	for (iter = lst.begin(); iter != lst.end(); ++iter)
	{
		printf("%d ", *iter);
	}
	printf("\n");

	return 0;
}