#include <cstdio>
#include <set>
using namespace std;

// ���� �����̳� : set, map, multiset, multimap
// �ߺ�X, ����X : set, map
// �ߺ���� : multiset, multimap
// ���� ���� Ʈ��

// set
void main251()
{
	set<int> s;
	
	// ��� �Լ�
	/*
		clear() : ��� ���� ����
		count(k) : k�� ������ ��ȯ
		empty() : ������� ����
		erase(k) : ���� ����
		find(k) : ���� ��ġ ��ȯ
		insert(k) : ���� ����
		size() : ���� ����
		swap(s2) : ��ȯ
	*/
	
	// set�� insert�� ���� ���� ����
	s.insert(50);
	s.insert(30);
	s.insert(80);
	s.insert(40);
	s.insert(10);
	s.insert(70);
	s.insert(90);

	set<int>::iterator set_iter;				// ���� ������ less�� ����� �ݺ���
	for (set_iter = s.begin(); set_iter != s.end(); ++set_iter)
	{
		printf("%d ", *set_iter);
	}
	printf("\n");

	// �ߺ��� �Ұ����ϱ� ������ ����
	s.insert(50);
	s.insert(50);

	for (set_iter = s.begin(); set_iter != s.end(); ++set_iter)
	{
		printf("%d ", *set_iter);
	}
	printf("\n");

	printf("======================");
	// insert �������� ��ȯ
	pair<set<int>::iterator, bool> pr;

	pr = s.insert(20);
	if (pr.second == true) {
		printf("%d ���� ����", *pr.first);
	}
	else {
		printf("%d�� �̹� �ֽ��ϴ�. ���� ����!", *pr.first);
	}
	printf("\n");

	pr = s.insert(20);
	if (pr.second == true) {
		printf("%d ���� ����", *pr.first);
	}
	else {
		printf("%d�� �̹� �ֽ��ϴ�. ���� ����!", *pr.first);
	}
	printf("\n");
}