#include <iostream>
#include <queue>

using namespace std;

int mainQ() {
	queue<int> q;

	q.push(1);
	q.push(2);
	q.push(3);

	q.pop();

	while (!q.empty()) {
		cout << q.front() << endl;
		q.pop();
	}

	return 0;
}

// FIFO

/*
	- 방식 : FIFO -
	empty() : 비어있으면 true, 아니면 false
	size() : 원소수를 리턴
	front() : 맨 앞 원소를 리턴
	back() : 맨 뒤 원소를 리턴
	push(a) : 원소 a를 추가
	pop() : 원소를 삭제
*/