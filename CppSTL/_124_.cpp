#include <iostream>
#include <functional>
#include <string>

using namespace std;

/* # 1*/
template<typename T>
void Copy(T t, T s, int n)
{
	for (int i = 0; i < n; i++)
	{
		*(t + i) = *(s + i);
	}
}

class MyType
{
};

/* #2 */
template<typename T>
class MyStack
{
	T m_stack[100];
	int idx = 0;
public:
	void Push(T a_t)
	{
		m_stack[idx++] = a_t;
	}
	T Pop()
	{
		return m_stack[--idx];
	}
	int Empty()
	{
		return !(idx);
	}
};

template<typename T>
class MyQueue
{
	T m_queue[100];
	int idx;
public:
	MyQueue() : idx(0) {}
	void Push(T a_t)
	{
		if(idx == 0){
			m_queue[0] = a_t;
			idx++;
		}
		else
		{
			for (int i = idx; i > 0; i--) {
				m_queue[i] = m_queue[i-1];
			}
			m_queue[0] = a_t;
			idx++;
		}
	}

	T Pop()
	{
		return m_queue[--idx];
	}

	int Empty()
	{
		return !(idx);
	}
};

int main124()
{
	// #1
	int arr1[5] = { 10, 20, 30, 40, 50 };
	int arr2[5];

	Copy(arr2, arr1, 5);

	for (int i = 0; i < 5; i++) {
		cout << arr2[i] << endl;
	}
	cout << endl;

	MyType myArr1[5];
	MyType myArr2[5];
	Copy(myArr2, myArr1, 5);

	// #2
	MyStack<int> st;
	st.Push(10);
	st.Push(20);
	st.Push(30);

	if (!st.Empty()) {
		cout << st.Pop() << endl;					// 30
	}
	if (!st.Empty()) {
		cout << st.Pop() << endl;					// 20
	}
	if (!st.Empty()) {
		cout << st.Pop() << endl;					// 10
	}
	cout << endl;

	// #3
	MyQueue<int> q;

	q.Push(10);
	q.Push(20);
	q.Push(30);

	if (!q.Empty()) {
		cout << q.Pop() << endl;				// 10
	}
	if (!q.Empty()) {
		cout << q.Pop() << endl;				// 20
	}
	if (!q.Empty()) {
		cout << q.Pop() << endl;				// 30
	}

	return 0;
}

