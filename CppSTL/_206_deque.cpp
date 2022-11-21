// deque컨테이너
#include <iostream>
#include <deque>
using namespace std;

int main206() {
	deque<int> dq;
	for (int i = 0; i < 10; ++i)
	{
		dq.push_back((i + 1) * 10);
	}

	dq.push_front(111);
	dq.push_front(222);

	for (int i = 0; i < dq.size(); ++i)
	{
		cout << dq[i] << ' ';
	}
	cout << endl;

	return 0;
}
