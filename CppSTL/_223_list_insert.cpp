#include <cstdio>
#include <vector>
#include <list>

// 리스트와 벡터의 insert는 결과는 같으나 과정이 다르다
// 리스트는 노드 삽입만 하고
// 백터는 노드 삽입 후 한칸씩 밀어낸다

using namespace std;

int main223()
{
	vector<int> v;
	list<int> lst;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	lst.push_back(10);
	lst.push_back(20);
	lst.push_back(30);
	lst.push_back(40);
	lst.push_back(50);

	vector<int>::iterator v_iter = v.begin();
	++v_iter;				// 20 원소를 가리킴
	list<int>::iterator lst_iter = lst.begin();
	++lst_iter;				// 20 원소를 가리킴

	v_iter = v.insert(v_iter, 600);					// v의 두번째 요소로 삽입
	lst_iter = lst.insert(lst_iter, 600);			// lst의 두번째 요소로 삽입

	printf("vector: %d \n", *v_iter);
	printf("list: %d \n", *lst_iter);

	printf("vector : ");
	for (v_iter = v.begin(); v_iter != v.end(); ++v_iter)
	{
		printf("%d ", *v_iter);
	}
	printf("\n");

	printf("list : ");
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	return 0;
}