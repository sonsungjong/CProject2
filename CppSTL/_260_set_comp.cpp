#include <cstdio>
#include <set>
#include <typeinfo>

using namespace std;

int main260()
{
	set<int, less<int>> s_less;							// set<int> s_less; �� ����
	set<int, greater<int>> s_greater;						// set�� ���������� �ο�

	s_less.insert(50);
	s_less.insert(80);
	s_less.insert(40);

	s_greater.insert(50);
	s_greater.insert(80);
	s_greater.insert(40);

	set<int, less<int>>::key_compare less_cmp = s_less.key_comp();								// �����ڸ� ��ȯ
	printf("%d\n", less_cmp(10, 20));						// 10 < 20
	
	set<int, greater<int>>::key_compare greater_cmp = s_greater.key_comp();
	printf("%d\n", greater_cmp(10, 20));						// 10 > 20 ����

	// typeid().name() : Ÿ���� ���ڿ��� ��ȯ
	printf("key_compare type: %s\n", typeid(s_less.key_comp()).name());
	printf("key_compare type: %s\n", typeid(s_greater.key_comp()).name());

	// set�� key�� �� value�� key_comp()�� value_comp() �� ����.
	printf("key_compare type: %s\n", typeid(s_less.value_comp()).name());
	printf("key_compare type: %s\n", typeid(s_greater.value_comp()).name());

	return 0;
}