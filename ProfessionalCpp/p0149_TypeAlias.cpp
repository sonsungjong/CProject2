import "HeaderUnits.h";

#include <vector>

// type alias �� ���� ����� Ÿ�Կ� �ٸ� �̸��� ���̴� ���̴� (���Ǿ�)
// Ÿ�� �ٸ���� C++11 ���� ���ԵǾ��� �������� typedef ���� �������

using IntPtr = int*;				// ���� Ÿ�� ǥ�� ��� ����� �� �ְ� �ȴ� (Ÿ�� �ٸ��)
typedef float* FloatPtr;				// Ÿ�� ������

using string = basic_string<char>;				// �� Ÿ�� �̸��� ���δ�

int main0149() {
	int* p1;
	IntPtr p2;

	vector<basic_string<char>> myVector;
	string str;


	return 0;
}