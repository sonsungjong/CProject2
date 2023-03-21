#include <forward_list>

int main045()
{
	std::forward_list<int> fwd_list = { 1,2,3 };
	fwd_list.push_front(0);				// 맨 앞에 0 추가 {0,1,2,3}
	auto it = fwd_list.begin();
	fwd_list.insert_after(it, 5);			// 맨 처음 원소 뒤에 5 추가 {0,5,1,2,3}
	fwd_list.insert_after(it, 6);			// 맨 처음 원소 뒤에 6 추가 {0,6,5,1,2,3}

	fwd_list.pop_front();				// 맨 앞의 원소를 제거 {6,5,1,2,3}
	fwd_list.erase_after(it);				// 맨 앞 다음의 원소를 제거 {6,1,2,3}

	return 0;
}