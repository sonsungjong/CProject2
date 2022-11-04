#include <iostream>
#include <vector>
#include <stack>
using namespace std;

// ������ �ݺ��� : �ݴ�� ���� (--, ++)

int main149()
{
	stack<int, vector<int>> st;						// vector �ڷᱸ���� �̿��� stack �ڷᱸ�� ����
	
	st.push(10);
	st.push(20);
	st.push(30);
	
	cout << st.top() << endl;
	st.pop();
	cout << st.top() << endl;
	st.pop();
	cout << st.top() << endl;
	st.pop();
	
	if (st.empty()) {
		cout << "stack�� ������ ����" << endl;
	}
	cout << endl;

	// reverse_iterator

	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for (vector<int>::iterator iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;

	// ������ �ݺ���
	reverse_iterator<vector<int>::iterator> riter(v.end());
	reverse_iterator<vector<int>::iterator> end_riter(v.begin());

	for (; riter != end_riter; ++riter)
	{
		cout << *riter << " ";
	}
	cout << endl;

	return 0;
}