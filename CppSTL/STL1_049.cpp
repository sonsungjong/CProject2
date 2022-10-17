#include <iostream>
using namespace std;

// explicit : 생성자의 호출을 명시적 호출만 가능하게 만들어 의도치않은 생성자 형변환을 막음 (생성자가 인자를 갖을 때 생성자 앞에다가 사용)

class Point
{
	int x;
	int y;
public:
	// 생성자 이름 앞에 explicit을 붙여주면 명시적 생성자호출만 가능함
	// 인자를 갖는 생성자 앞에는 explicit을 붙여주어 암시적 생성자 형변환을 막아주는 것이 좋음 (명시적 호출만 가능해짐)
	explicit Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const { cout << x << ',' << y << endl; }
};

int main049()
{
	Point pt;
	pt.Print();

	// 암시적 생성자 호출 (explicit 을 붙여주면 불가능)
	// pt = 10;				// Point(10, 0);

	pt.Print();

	return 0;
}
