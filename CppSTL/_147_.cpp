#include <iostream>
#include <stack>
using namespace std;

int main147()
{
	stack<int> st;					// stack�����̳� ����
	st.push(10);					// ������ �ױ�
	st.push(20);
	st.push(30);

	cout << st.top() << endl;					// top ������ ��ȸ
	st.pop();											// top ������ ����
	cout << st.top() << endl;
	st.pop();
	cout << st.top() << endl;
	st.pop();

	if (st.empty()) {
		cout << "stack�� ������ ����" << endl;
	}

	return 0;
}

// stack : �ױ�
// push, pop, top, empty