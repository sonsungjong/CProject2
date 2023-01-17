#include <cstdio>
#include <map>
using namespace std;

// map 컨테이너는 key와 value를 쌍으로 저장한다. (pair)
// key는 중복되지 않는다

int main276()
{
	map<int, int> m;

	m.insert(pair<int, int>(5, 100));							// 임시 pair로 삽입
	m.insert(pair<int, int>(3, 100));
	m.insert(pair<int, int>(8, 30));
	m.insert(pair<int, int>(4, 40));
	m.insert(pair<int, int>(1, 70));
	m.insert(pair<int, int>(7, 100));

	pair<int, int> pr(9, 50);
	m.insert(pr);							// pair 객체 생성 후 삽입

	map<int, int>::iterator map_iter;
	for (map_iter = m.begin(); map_iter != m.end(); ++map_iter)
	{
		printf("(%d, %d) ", (*map_iter).first, (*map_iter).second);
	}
	printf("\n");

	for (map_iter = m.begin(); map_iter != m.end(); ++map_iter)
	{
		printf("(%d, %d) ", map_iter->first, map_iter->second);
	}
	printf("\n");

	return 0;
}