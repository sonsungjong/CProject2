#include <iostream>
#include <algorithm>
using namespace std;

// Ŭ���̾�Ʈ
struct Functor4
{
	void operator()(int n)
	{
		cout << n << " ";
	}
};

struct Functor5
{
	void operator()(int n)
	{
		cout << n * n << " ";
	}
};

struct Functor6
{
	void operator()(int n)
	{
		cout << "���� : " << n << endl;
	}
};

int main80()
{
	int arr[5] = { 10, 20, 30, 40, 50 };
	
	for_each(arr, arr + 5, Functor4());
	// �ӽ� �Լ� ��ü Functor4()�� ����� �Լ��� ����

	cout << endl << endl;
	for_each(arr, arr + 5, Functor5());

	cout << endl << endl;
	for_each(arr, arr + 5, Functor6());

	return 0;
}