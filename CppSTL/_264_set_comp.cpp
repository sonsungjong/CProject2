#include <cstdio>
#include <set>
using namespace std;

int main264()
{
	set<int, less<int>> s;

	// 30과 50 비교
	int result = !s.key_comp()(30, 50) && !s.key_comp()(50, 30);		// 30이 50보다 앞서있지 않고, 50도 30보다 앞서있지 않은가? false
	printf("%d\n", result);

	// 30과 30 비교
	result = !s.key_comp()(30, 30) && !s.key_comp()(30, 30);			// 30이 30보다 앞서있지 않고 30도 30보다 앞서있지 않은가? true
	printf("%d\n", result);

	return 0;
}