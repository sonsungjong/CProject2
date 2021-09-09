#include <iostream>
#include <stack>
using namespace std;

int main528()
{
	stack<int> st;

	st.push(10);				// 데이터 입력
	st.push(20);
	st.push(30);

	cout << st.top() << endl;			// 최상단 데이터값 출력
	st.pop();					// 최상단 데이터 삭제

	cout << st.top() << endl;
	st.pop();

	cout << st.top() << endl;
	st.pop();

	if (st.empty()) {
		cout << "stack 데이터 없음" << endl;
	}

	/*
	while (!st.empty()) {
		cout << st.top() << endl;
		st.pop();
	}
	
	cout << "stack 데이터 없음" << endl;
	*/

	return 0;
}