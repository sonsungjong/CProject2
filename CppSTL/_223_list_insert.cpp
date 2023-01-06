#include <cstdio>
#include <vector>
#include <list>

// ����Ʈ�� ������ insert�� ����� ������ ������ �ٸ���
// ����Ʈ�� ��� ���Ը� �ϰ�
// ���ʹ� ��� ���� �� ��ĭ�� �о��

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
	++v_iter;				// 20 ���Ҹ� ����Ŵ
	list<int>::iterator lst_iter = lst.begin();
	++lst_iter;				// 20 ���Ҹ� ����Ŵ

	v_iter = v.insert(v_iter, 600);					// v�� �ι�° ��ҷ� ����
	lst_iter = lst.insert(lst_iter, 600);			// lst�� �ι�° ��ҷ� ����

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