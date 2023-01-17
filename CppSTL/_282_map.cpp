#include <cstdio>
#include <map>

using namespace std;

int main282()
{
	map<int, int> m;

	// 대입
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

	// 수정
	m[5] = 200;

	for (map_iter = m.begin(); map_iter != m.end(); ++map_iter)
	{
		printf("(%d, %d) ", map_iter->first, map_iter->second);
	}
	printf("\n");

	return 0;
}