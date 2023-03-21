#include <vector>

int main040()
{
	std::vector<int> vec;				// 크기가 0인 벡터

	std::vector<int> vec1 = { 1,2,3,4,5 };			// 지정한 초기값으로 이루어진 크기가 5인 벡터

	std::vector<int> vec2(10);				// 크기가 10인 벡터

	std::vector<int> vec3(10, 5);				// 크기가 10이고 모든 원소가 5인 벡터

	// insert : 원하는 위치에 값을 삽입
	// push_back : 뒤쪽에 값을 대입
	// pop_back : 뒤쪽에 값을 제거
	// erase : 원하는 위치에 값을 제거
	// clear : 모두 제거
	// reserve : 사용할 용량 지정
	// shrink_to_fit : 여분의 메모리 공간 제거

	return 0;
}