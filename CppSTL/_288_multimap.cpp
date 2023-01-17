#include <cstdio>
#include <map>
using namespace std;

// multimap : key�ߺ��� ������ map

int main288()
{
	multimap<int, int> mm;

	mm.insert(pair<int, int>(5, 100));
	mm.insert(pair<int, int>(3, 100));
	mm.insert(pair<int, int>(8, 30));
	mm.insert(pair<int, int>(3, 40));
	mm.insert(pair<int, int>(1, 100));
	mm.insert(pair<int, int>(7, 100));
	mm.insert(pair<int, int>(8, 100));

	multimap<int, int>::iterator mm_iter;
	for (mm_iter = mm.begin(); mm_iter != mm.end(); ++mm_iter)
	{
		printf("(%d, %d) ", mm_iter->first, mm_iter->second);
	}
	printf("\n");

	// count()
	printf("key 3�� ������ ������ %lld�� \n", mm.count(3));

	// find()
	mm_iter = mm.find(3);
	if (mm_iter != mm.end())
	{
		printf("ù ��° key 3�� ���ε� value: %d\n", mm_iter->second);
	}
	return 0;
}