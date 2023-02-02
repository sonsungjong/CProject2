#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

class PrintFunctor318
{
private:
	char fmt;
public:
	// 왜 상속을 통해 초기화를 해야 띄어쓰기가 되는지 모르겠음
	explicit PrintFunctor318(char c= ' ') : fmt(c)
	{
	}
	void operator() (int n) const
	{
		printf("%d%c", n, fmt);
	}
};

int main318()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for_each(v.begin(), v.end(), PrintFunctor318());				// 원소를 ' '로 구분
	printf("\n");
	for_each(v.begin(), v.end(), PrintFunctor318(','));				// 원소를 ',' 로 구분
	printf("\n");
	for_each(v.begin(), v.end(), PrintFunctor318('\n'));				// 원소를 '\n'로 구분
	printf("\n");

	return 0;
}