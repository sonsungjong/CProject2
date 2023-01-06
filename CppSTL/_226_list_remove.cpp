#include <cstdio>
#include <list>

using namespace std;
// remove : �ش� ���Ҹ� �����ִ� ��� ��带 ���� (�����ϰ� �ݺ��ڰ� �ʿ����, �ӵ��� ����)
// remove_if : �Լ������͸� ���ؼ� ���ǿ� �ش��ϴ� ���Ҹ� ��� ����

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

	// remove ��
	for (iter = lst.begin(); iter != lst.end(); ++iter)
	{
		printf("%d ", *iter);
	}
	printf("\n");

	// remove ��
	lst.remove(10);				// 10 ������ ��带 ��� ����
	for (iter = lst.begin(); iter != lst.end(); ++iter) 
	{
		printf("%d ", *iter);
	}
	printf("\n");

	// remove_if
	lst.remove_if(Predicate);					// �����ڰ� ���� ��� ���Ҹ� ����
	for (iter = lst.begin(); iter != lst.end(); ++iter)
	{
		printf("%d ", *iter);
	}
	printf("\n");

	return 0;
}