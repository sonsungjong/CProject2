#include <cstdio>
#include <set>

using namespace std;

int main265()
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
	for (set_iter = s.begin(); set_iter != s.end(); ++set_iter)
	{
		printf("%d ", *set_iter);
	}
	printf("\n");

	set<int>::iterator set_iter_lower;
	set<int>::iterator set_iter_upper;

	// lower_bound 와 upper_bound
	set_iter_lower = s.lower_bound(30);						// 30이 시작하는 구간의 반복자
	set_iter_upper = s.upper_bound(30);						// 30이 끝나는 구간의 반복자
	printf("%d\n", *set_iter_lower);
	printf("%d\n", *set_iter_upper);

	set_iter_lower = s.lower_bound(55);					// 55가 시작하는 구간의 반복자
	set_iter_upper = s.upper_bound(55);					// 55가 끝나는 구간의 반복자
	if (set_iter_lower == set_iter_upper)					// 두 반복자가 같다면 찾는 원소는 없음
	{
		printf("55가 s에 없음...\n");
	}
	else if(set_iter_lower != set_iter_upper){				// lower_bound 와 upper_bound가 다르면 찾는 원소가 있음
		printf("55가 s에 있음!\n");
	}
	printf("\n");

	// equal_range : lower_bound 와 upper_bound를 합침
	pair<set<int>::iterator, set<int>::iterator> set_iter_pair;
	set_iter_pair = s.equal_range(30);

	printf("%d\n", *set_iter_pair.first);
	printf("%d\n", *set_iter_pair.second);

	set_iter_pair = s.equal_range(55);
	if (set_iter_pair.first == set_iter_pair.second)								// 반복자 lower_bound 와 upper_bound가 같으면 찾는 원소가 없음
	{
		printf("55가 s에 없음... \n");
	}
	else if(set_iter_pair.first != set_iter_pair.second){							// lower_bound 와 upper_bound가 다르면 찾는 원소가 있음
		printf("55가 s에 있음! \n");
	}

	return 0;
}