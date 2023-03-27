#include <iostream>
#include <deque>
#include <queue>

int main066()
{
	std::deque<int> deq = { 1,2,3,4,5 };

	deq.push_front(0);				// 0, 1,2,3,4,5

	deq.push_back(6);				// 0, 1,2,3,4,5,6

	deq.insert(deq.begin() + 2, 10);				// 0, 1,10,2,3,4,5, 6

	deq.pop_back();				// 0,1,10,2,3,4, 5

	deq.pop_front();				// 1, 10, 2, 3, 4, 5

	deq.erase(deq.begin() + 1);				// 1, 2, 3, 4, 5

	deq.erase(deq.begin() + 3, deq.end());				// 1, 2, 3

	std::queue<int> q;

	// 나중으로 추가
	q.push(1);				// 1
	q.push(2);				// 1, 2
	q.push(3);				// 1, 2, 3
	// 먼저 들어온 요소 제거
	q.pop();					// 2, 3
	q.push(4);				// 2, 3, 4

	return 0;
}