#include <iostream>
#include <algorithm>
using namespace std;

// Ŭ���̾�Ʈ1
void _MyPrint1(int n)
{
	cout << n << ' ';
}

void _MyPrint2(int n)
{
	cout << n * n << " ";
}

void _MyPrint3(int n)
{
	cout << "���� : " << n << endl;
}

int main071()
{
	int arr[5] = { 10, 20, 30, 40, 50 };

	for_each(arr, arr + 5, _MyPrint1);
	cout << endl << endl;

	for_each(arr, arr + 5, _MyPrint2);
	cout << endl << endl;
	for_each(arr, arr + 5, _MyPrint3);				// �Լ��� �ּҸ� �Ű������� ����

	return 0;
}