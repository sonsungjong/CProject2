#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

// find_end() : 

int main309()
{
	vector<int> v1;
	vector<int> v2;

	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);
	v1.push_back(40);
	v1.push_back(50);
	v1.push_back(60);
	v1.push_back(70);
	v1.push_back(30);
	v1.push_back(40);
	v1.push_back(50);

	v2.push_back(30);
	v2.push_back(40);
	v2.push_back(50);

	vector<int>::iterator iter;
	iter = find_end(v1.begin(), v1.end(), v2.begin(), v2.end());
	if (iter != v1.end())
	{
		printf("iter : %d\n", *iter);
		printf("iter-1 : %d\n", *(iter-1));
		printf("iter+1 : %d\n", *(iter+1));
	}

	return 0;
}