#include <iostream>
#include <vector>
#include <stack>
using namespace std;

int main529()
{
	stack<int, vector<int>> st;			// vector를 이용한 stack

	st.push(10);
	st.push(20);
	st.push(30);

	while (true)
	{
		if (st.empty()) {
			cout << "stack에 데이터 없음" << endl;
			break;
		}
		cout << st.top() << endl;
		st.pop();
	}

	return 0;
}