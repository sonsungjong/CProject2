#include <iostream>
#include <stack>
using namespace std;

int main528()
{
	stack<int> st;

	st.push(10);				// ������ �Է�
	st.push(20);
	st.push(30);

	cout << st.top() << endl;			// �ֻ�� �����Ͱ� ���
	st.pop();					// �ֻ�� ������ ����

	cout << st.top() << endl;
	st.pop();

	cout << st.top() << endl;
	st.pop();

	if (st.empty()) {
		cout << "stack ������ ����" << endl;
	}

	/*
	while (!st.empty()) {
		cout << st.top() << endl;
		st.pop();
	}
	
	cout << "stack ������ ����" << endl;
	*/

	return 0;
}