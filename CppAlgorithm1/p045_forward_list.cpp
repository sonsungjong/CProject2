#include <forward_list>

int main045()
{
	std::forward_list<int> fwd_list = { 1,2,3 };
	fwd_list.push_front(0);				// �� �տ� 0 �߰� {0,1,2,3}
	auto it = fwd_list.begin();
	fwd_list.insert_after(it, 5);			// �� ó�� ���� �ڿ� 5 �߰� {0,5,1,2,3}
	fwd_list.insert_after(it, 6);			// �� ó�� ���� �ڿ� 6 �߰� {0,6,5,1,2,3}

	fwd_list.pop_front();				// �� ���� ���Ҹ� ���� {6,5,1,2,3}
	fwd_list.erase_after(it);				// �� �� ������ ���Ҹ� ���� {6,1,2,3}

	return 0;
}