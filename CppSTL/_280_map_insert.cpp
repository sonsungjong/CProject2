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

	pr = m.insert(pair<int, int>(9, 50));					// 성공
	if (true == pr.second)
	{
		printf("key: %d, value: %d 저장 완료!\n", pr.first->first, pr.first->second);
	}
	else {
		printf("key 9가 이미 m에 있습니다.\n");
	}

	pr = m.insert(pair<int, int>(9, 50));					// 실패
	if (true == pr.second)
	{
		printf("key: %d, value: %d 저장 완료!\n", pr.first->first, pr.first->second);
	}
	else {
		printf("key 9가 이미 m에 있습니다.\n");
	}


	return 0;
}