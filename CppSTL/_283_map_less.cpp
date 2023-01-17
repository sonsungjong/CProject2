#include <cstdio>
#include <map>
#include <string>
using namespace std;

int main283()
{
	map<int, string, less<int>> m;							// 기본 정렬 기준

	map<int, string, greater<int>> m_greater;				// key 정렬 기준을 내림차순으로 변경

	m_greater[5] = "five";
	m_greater[3] = "three";
	m_greater[8] = "eight";
	m_greater[4] = "four";
	m_greater[1] = "one";
	m_greater[7] = "seven";
	m_greater[9] = "nine";

	map<int, string, greater<int>>::iterator map_iter;
	for (map_iter = m_greater.begin(); map_iter != m_greater.end(); ++map_iter)
	{
		printf("(%d, %s) ", map_iter->first, map_iter->second);
	}
	printf("\n");

	printf("%s ", m_greater[9]);
	printf("%s ", m_greater[8]);
	printf("%s ", m_greater[7]);
	printf("%s ", m_greater[5]);
	printf("%s ", m_greater[4]);
	printf("%s ", m_greater[3]);
	printf("%s ", m_greater[1]);

	return 0;
}