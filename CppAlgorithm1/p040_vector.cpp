#include <vector>

int main040()
{
	std::vector<int> vec;				// ũ�Ⱑ 0�� ����

	std::vector<int> vec1 = { 1,2,3,4,5 };			// ������ �ʱⰪ���� �̷���� ũ�Ⱑ 5�� ����

	std::vector<int> vec2(10);				// ũ�Ⱑ 10�� ����

	std::vector<int> vec3(10, 5);				// ũ�Ⱑ 10�̰� ��� ���Ұ� 5�� ����

	// insert : ���ϴ� ��ġ�� ���� ����
	// push_back : ���ʿ� ���� ����
	// pop_back : ���ʿ� ���� ����
	// erase : ���ϴ� ��ġ�� ���� ����
	// clear : ��� ����
	// reserve : ����� �뷮 ����
	// shrink_to_fit : ������ �޸� ���� ����

	return 0;
}