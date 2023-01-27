#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

bool Pred307(int n)
{
	return n > 35;
}

int main307()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for (vector<int>::size_type i = 0; i < v.size(); ++i)
	{
		printf("%d ", v[i]);
	}
	printf("\n");

	vector<int>::iterator iter;
	// find �� �������� 20 ã��
	iter = find(v.begin(), v.end(), 20);
	if (iter != v.end()) {
		printf("%d �� ã�Ҵ� \n", *iter);
	}

	// find_if �� �������� 35���� ū ù ���� ã��
	iter = find_if(v.begin(), v.end(), Pred307);
	if (iter != v.end()) {
		printf("���������� 35���� ū ù��° ����: %d \n", *iter);
	}

	return 0;
}