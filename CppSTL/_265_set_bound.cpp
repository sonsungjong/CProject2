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

	// lower_bound �� upper_bound
	set_iter_lower = s.lower_bound(30);						// 30�� �����ϴ� ������ �ݺ���
	set_iter_upper = s.upper_bound(30);						// 30�� ������ ������ �ݺ���
	printf("%d\n", *set_iter_lower);
	printf("%d\n", *set_iter_upper);

	set_iter_lower = s.lower_bound(55);					// 55�� �����ϴ� ������ �ݺ���
	set_iter_upper = s.upper_bound(55);					// 55�� ������ ������ �ݺ���
	if (set_iter_lower == set_iter_upper)					// �� �ݺ��ڰ� ���ٸ� ã�� ���Ҵ� ����
	{
		printf("55�� s�� ����...\n");
	}
	else if(set_iter_lower != set_iter_upper){				// lower_bound �� upper_bound�� �ٸ��� ã�� ���Ұ� ����
		printf("55�� s�� ����!\n");
	}
	printf("\n");

	// equal_range : lower_bound �� upper_bound�� ��ħ
	pair<set<int>::iterator, set<int>::iterator> set_iter_pair;
	set_iter_pair = s.equal_range(30);

	printf("%d\n", *set_iter_pair.first);
	printf("%d\n", *set_iter_pair.second);

	set_iter_pair = s.equal_range(55);
	if (set_iter_pair.first == set_iter_pair.second)								// �ݺ��� lower_bound �� upper_bound�� ������ ã�� ���Ұ� ����
	{
		printf("55�� s�� ����... \n");
	}
	else if(set_iter_pair.first != set_iter_pair.second){							// lower_bound �� upper_bound�� �ٸ��� ã�� ���Ұ� ����
		printf("55�� s�� ����! \n");
	}

	return 0;
}