#include <cstdio>
#include <map>
using namespace std;

int main280()
{
	map<int, int> m;
	pair<map<int, int>::iterator, bool> pr;

	m.insert(pair<int, int>(5, 100));
	m.insert(pair<int, int>(3, 100));
	m.insert(pair<int, int>(8, 30));
	m.insert(pair<int, int>(4, 40));
	m.insert(pair<int, int>(1, 70));
	m.insert(pair<int, int>(7, 100));

	pr = m.insert(pair<int, int>(9, 50));					// ����
	if (true == pr.second)
	{
		printf("key: %d, value: %d ���� �Ϸ�!\n", pr.first->first, pr.first->second);
	}
	else {
		printf("key 9�� �̹� m�� �ֽ��ϴ�.\n");
	}

	pr = m.insert(pair<int, int>(9, 50));					// ����
	if (true == pr.second)
	{
		printf("key: %d, value: %d ���� �Ϸ�!\n", pr.first->first, pr.first->second);
	}
	else {
		printf("key 9�� �̹� m�� �ֽ��ϴ�.\n");
	}


	return 0;
}