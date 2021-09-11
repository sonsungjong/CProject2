#include <iostream>
#include <deque>
using namespace std;

int main6201()
{
	deque<int> dq;


	return 0;
}

/*
	vector : 원소를 추가할 때마다 실시간으로 메모리를 확보해서 추가하는 순차배열, 원소에 빠르게 접근할 수 있지만 원소를 추가할 때 메모리 재할당하고 원소를 복제하여 성능저하가 발생
	deque : 미리 메모리 블록을 할당하고 원소를 추가, 메모리가 부족할 때는 이전 메모리와 원소를 모두 제거하고 새로운 메모리 블록을 할당
*/