import "HeaderUnits.h";
#include <compare>

using namespace std;

int main0087()
{
	int i{ 11 };
	std::strong_ordering result{ i <=> 0 };				// 3방향 비교 연산자
	if (result == strong_ordering::less) { printf("less \n"); }
	if (result == strong_ordering::greater) { printf("greater \n"); }
	if (result == strong_ordering::equal) { printf("equal \n"); }

	int j{ 11 };
	strong_ordering result2{ j <=> 0 };
	if (is_lt(result2)) { printf("less \n"); }
	if (is_gt(result2)) { printf("greater \n"); }
	if (is_eq(result2)) { printf("equal \n"); }
}
