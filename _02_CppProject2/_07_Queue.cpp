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
	- ��� : FIFO -
	empty() : ��������� true, �ƴϸ� false
	size() : ���Ҽ��� ����
	front() : �� �� ���Ҹ� ����
	back() : �� �� ���Ҹ� ����
	push(a) : ���� a�� �߰�
	pop() : ���Ҹ� ����
*/