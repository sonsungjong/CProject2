import "HeaderUnits.h";

#include <vector>

// type alias 란 기존 선언된 타입에 다른 이름을 붙이는 것이다 (동의어)
// 타입 앨리어스는 C++11 부터 도입되었고 그전에는 typedef 으로 사용했음

using IntPtr = int*;				// 기존 타입 표현 대신 사용할 수 있게 된다 (타입 앨리어스)
typedef float* FloatPtr;				// 타입 디파인

using string = basic_string<char>;				// 긴 타입 이름을 줄인다

int main0149() {
	int* p1;
	IntPtr p2;

	vector<basic_string<char>> myVector;
	string str;


	return 0;
}