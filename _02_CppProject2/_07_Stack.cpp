#include <iostream>
#include <stack>

using namespace std;

int mainS() {
	stack<int> _stack;

	_stack.push(1);
	_stack.push(2);
	_stack.push(3);

	// top
	//cout << s.top() << endl;

	_stack.pop();

	//cout << s.size() << endl;

	while (!_stack.empty()) {
		cout << _stack.top() << endl;
		_stack.pop();
	}

	return 0;
}

// LIFO

// STL ǥ�ض��̺귯�� �迭

/*
	- ��� : LIFO -
	empty() : ���Ұ� ��������� true, �ƴϸ� false
	size() : ���� ���� ����
	top() : ���� ����� ���Ҹ� ����
	push(a) : ���ÿ� a ���Ҹ� �߰�
	pop() : ���ÿ� ���Ҹ� ����
*/