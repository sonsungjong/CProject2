#include <cstdio>
#include <map>
using namespace std;

int main284()
{
	map<int, int> m;

	m[5] = 100;
	m[3] = 100;
	m[8] = 30;
	m[4] = 40;
	m[1] = 70;
	m[7] = 100;
	m[9] = 50;

	map<int, int>::iterator map_iter;
	for (map_iter = m.begin(); map_iter != m.end(); ++map_iter)
	{
		printf("(%d, %d) ", map_iter->first, map_iter->second);
	}
	printf("\n");

	// find() �� ã��
	map_iter = m.find(5);
	if (map_iter != m.end()){
		printf("key 5�� ���ε� value: %d\n", map_iter->second);
	}

	// lower_bound(), upper_bound() �� ã��
	map<int, int>::iterator lower_iter = m.lower_bound(5);
	map<int, int>::iterator upper_iter = m.upper_bound(5);

	printf("���� [lower_iter, upper_iter)�� ������: ");
	for (map_iter = lower_iter; map_iter != upper_iter; ++map_iter)
	{
		printf("(%d, %d) ", map_iter->first, map_iter->second);
	}
	printf("\n");

	// equal_range() �� ã��
	pair<map<int, int>::iterator, map<int, int>::iterator> iter_pair;
	iter_pair = m.equal_range(5);

	printf("���� [iter_pair.first, iter_pair.second)�� ������: ");
	for (map_iter = iter_pair.first; map_iter != iter_pair.second; ++map_iter)
	{
		printf("(%d, %d) ", map_iter->first, map_iter->second);
	}
	printf("\n");

	return 0;
}