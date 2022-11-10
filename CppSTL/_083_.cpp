#include <iostream>
#include <functional>
using namespace std;

// Ÿ������ (�̸��� ��)
typedef less<int> Less;
typedef greater<int> Greater;

int main83()
{
	Less o_less;

	cout << o_less(10, 20) << endl;				// ��ü�� �Լ��� ȣ��
	cout << o_less(20, 10) << endl;
	cout << o_less.operator()(10, 20) << endl;
	cout << endl;

	cout << Less()(10, 20) << endl;				// �ӽð�ü�� �Լ��� ȣ��
	cout << Less()(20, 10) << endl;
	cout << Less().operator()(10, 20) << endl;

	cout << Greater()(10, 20) << endl;				// �ӽð�ü�� �Լ��� ȣ��
	cout << Greater()(20, 10) << endl;

	cout << endl;
	// STL functional�� less�� greater���
	cout << less<int>()(10, 20) << endl;
	cout << less<int>()(20, 10) << endl;
	cout << greater<int>()(10, 20) << endl;
	cout << greater<int>()(20, 10) << endl;

	return 0;
}