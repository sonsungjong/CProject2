#include <cstdio>
#include <set>
#include <typeinfo>

using namespace std;

int main260()
{
	set<int, less<int>> s_less;							// set<int> s_less; 와 같음
	set<int, greater<int>> s_greater;						// set에 정렬조건을 부여

	s_less.insert(50);
	s_less.insert(80);
	s_less.insert(40);

	s_greater.insert(50);
	s_greater.insert(80);
	s_greater.insert(40);

	set<int, less<int>>::key_compare less_cmp = s_less.key_comp();								// 조건자를 반환
	printf("%d\n", less_cmp(10, 20));						// 10 < 20
	
	set<int, greater<int>>::key_compare greater_cmp = s_greater.key_comp();
	printf("%d\n", greater_cmp(10, 20));						// 10 > 20 연산

	// typeid().name() : 타입을 문자열로 반환
	printf("key_compare type: %s\n", typeid(s_less.key_comp()).name());
	printf("key_compare type: %s\n", typeid(s_greater.key_comp()).name());

	// set은 key가 곧 value라서 key_comp()와 value_comp() 가 같다.
	printf("key_compare type: %s\n", typeid(s_less.value_comp()).name());
	printf("key_compare type: %s\n", typeid(s_greater.value_comp()).name());

	return 0;
}