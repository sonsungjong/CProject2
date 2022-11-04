#include <iostream>
#include <stack>
using namespace std;

int main147()
{
	stack<int> st;					// stack컨테이너 생성
	st.push(10);					// 데이터 쌓기
	st.push(20);
	st.push(30);

	cout << st.top() << endl;					// top 데이터 조회
	st.pop();											// top 데이터 삭제
	cout << st.top() << endl;
	st.pop();
	cout << st.top() << endl;
	st.pop();

	if (st.empty()) {
		cout << "stack에 데이터 없음" << endl;
	}

	return 0;
}

// stack : 쌓기
// push, pop, top, empty