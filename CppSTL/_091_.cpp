#include <iostream>
using namespace std;

// ���ø� : �Լ� ���ø�, Ŭ���� ���ø�
// 1. �Լ� ���ø� : �Լ��� ������ Ʋ
// 2. Ŭ���� ���ø� : Ŭ������ ������ Ʋ

// �ڷ����� ����ϴ� Ŭ���̾�Ʈ���� �����ϰԲ� ����
template<typename T>
void gPrint5(T a, T b)
{
	cout << a << ", " << b << endl;
}

int main91()
{
	// �Ͻ��� Ÿ������
	gPrint5(10, 20);
	gPrint5(0.123f, 1.123f);
	gPrint5("ABC", "abcde");

	// ����� Ÿ������
	gPrint5<int>(10, 20);
	gPrint5<float>(0.123f, 1.123f);
	gPrint5<const char*>("ABC", "abcde");

	return 0;
}