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

// STL 표준라이브러리 배열

/*
	- 방식 : LIFO -
	empty() : 원소가 비어있으면 true, 아니면 false
	size() : 원소 수를 리턴
	top() : 가장 상단의 원소를 리턴
	push(a) : 스택에 a 원소를 추가
	pop() : 스택에 원소를 제거
*/