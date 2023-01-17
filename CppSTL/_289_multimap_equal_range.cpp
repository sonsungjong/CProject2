#include <cstdio>
#include <map>
using namespace std;

int main289()
{
	multimap<int, int> mm;

	mm.insert(pair<int, int>(5, 100));
	mm.insert(pair<int, int>(3, 100));
	mm.insert(pair<int, int>(8, 30));
	mm.insert(pair<int, int>(3, 40));
	mm.insert(pair<int, int>(1, 70));
	mm.insert(pair<int, int>(7, 100));
	mm.insert(pair<int, int>(8, 50));
	
	// 3ã��
	multimap<int, int>::iterator mm_iter;
	// find() : ó�� �Ѱ��� ã����
	mm_iter = mm.find(3);
	if (mm_iter != mm.end()) {
		printf("ù��° key 3�� ���ε� value: %d\n", mm_iter->second);
	}

	// lower_bound(), upper_bound()
	multimap<int, int>::iterator lower_iter = mm.lower_bound(3);
	multimap<int, int>::iterator upper_iter = mm.upper_bound(3);
	
	printf("���� [lower_iter, upper_iter)�� ������: ");
	for (mm_iter = lower_iter; mm_iter != upper_iter; ++mm_iter) {
		printf("(%d, %d) ", mm_iter->first, mm_iter->second);
	}
	printf("\n");

	pair<multimap<int, int>::iterator, multimap<int, int>::iterator> iter_pair;
	iter_pair = mm.equal_range(3);

	printf("���� [iter_pair.first, iter_pair.second)�� ������: ");
	for (mm_iter = iter_pair.first; mm_iter != iter_pair.second; ++mm_iter)
	{
		printf("(%d, %d) ", mm_iter->first, mm_iter->second);
	}
	printf("\n");

	return 0;
}