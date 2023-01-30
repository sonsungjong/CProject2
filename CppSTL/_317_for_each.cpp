#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

void Print317(int n)
{
	printf("%d ", n);
}

int main()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for_each(v.begin(), v.end(), Print317);
	printf("\n");

	for_each(v.begin(), v.begin() + 2, Print317);
	printf("\n");
	for_each(v.begin(), v.begin() + 4, Print317);
	printf("\n");

	return 0;
}