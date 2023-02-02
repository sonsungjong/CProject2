#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

class PrintFunctor318
{
private:
	char fmt;
public:
	// �� ����� ���� �ʱ�ȭ�� �ؾ� ���Ⱑ �Ǵ��� �𸣰���
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

	for_each(v.begin(), v.end(), PrintFunctor318());				// ���Ҹ� ' '�� ����
	printf("\n");
	for_each(v.begin(), v.end(), PrintFunctor318(','));				// ���Ҹ� ',' �� ����
	printf("\n");
	for_each(v.begin(), v.end(), PrintFunctor318('\n'));				// ���Ҹ� '\n'�� ����
	printf("\n");

	return 0;
}