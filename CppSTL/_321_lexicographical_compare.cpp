#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

template<typename T>
class Less321
{
public:
	bool operator() (const T& left, const T& right) const
	{
		return left < right;				// '������ �۴�'�� ����
	}
};

int main321()
{
	vector<int> v1;
	vector<int> v2;

	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);

	v2.push_back(10);
	v2.push_back(25);
	v2.push_back(30);

	if (lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), less<int>())) {
		printf("���� less... v1�� v2�� ��: true \n");
	}
	else {
		printf("���� less... v1�� v2�� ��: false \n");
	}

	if (lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), greater<int>())) {
		printf("���� greater... v1�� v2�� ��: true \n");
	}
	else {
		printf("���� greater... v1�� v2�� ��: false \n");
	}

	if (lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), Less321<int>())) {
		printf("Less321 ������ ����... v1�� v2�� ��: true \n");
	}
	else {
		printf("Less321 ������ ����... v1�� v2�� �� : false \n");
	}

	return 0;
}