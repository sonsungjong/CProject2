#include <cstdio>
#include <set>

using namespace std;
// count() : �ش� ������ ������ ��ȯ
// find() : �ش� ���Ҹ� �˻��Ͽ� �� ��ġ�� ����Ű�� �ݺ��ڸ� ��ȯ

int main262()
{
	set<int> s;

	s.insert(50);
	s.insert(30);
	s.insert(80);
	s.insert(40);
	s.insert(10);
	s.insert(70);
	s.insert(90);

	set<int>::iterator set_iter;

	// count : ����
	for (set_iter = s.begin(); set_iter != s.end(); ++set_iter)
	{
		printf("%d ", *set_iter);
	}
	printf("\n");

	// count : �� ������ ������ ��ȯ
	printf("���� 50�� ����: %lld\n", s.count(50));
	printf("���� 100�� ����: %lld\n", s.count(100));

	// find : ã��
	set_iter = s.find(30);					// 30�� �ݺ��ڸ� ��ȯ (�ݺ��ڸ� �������Ͽ� ���)
	if (set_iter != s.end())
	{
		printf("%d�� s�� �ִ�.\n", *set_iter);
	}
	else {
		printf("�ش� ���� s�� ����.\n");
	}

	return 0;
}