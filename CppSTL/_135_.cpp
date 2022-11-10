// page 131
// STL : ǥ�� C++ ���̺귯���� �Ϻκ�, Standard Template Library
// �ڷᱸ�� + �˰��� + �ݺ��ڸ� ���ø����� �����ϴ� ���̺귯��

// 1. �����̳�(Container) : ��ü�� �����ϴ� ��ü, �÷��� �Ǵ� �ڷᱸ����� �Ҹ�
// 2. �ݺ���(Iterator) : �����Ϳ� ����� �������� �����̳��� ���Ҹ� ����Ű��, ����Ű�� ���ҿ� �����Ͽ� ���� ���Ҹ� ����Ű�� �ϴ� ����� �Ѵ�.
// 3. �˰���(Algorithm) : ����, ����, �˻�, ���� �� �Ϲ�ȭ�� ����� �����ϴ� �Լ� ���ø�
// 4. �Լ� ��ü(�Լ���, Function Object) : �Լ�ó�� �����ϴ� ��ü, operator() �� �����ε��Ͽ� �����Ѵ�. �ڷᱸ�� �� �˰����� Ŭ���̾�Ʈ ��å�� �ݿ��Ѵ�.
// 5. �����(Adaptor) : �������̽��� ������ ���ο� �������̽��� ���� ���� ��ҷ� ������
// 6. �Ҵ��(Allocator) : �޸� �Ҵ� ��å�� ĸ��ȭ�� Ŭ���� ��ü, ��� �����̳ʴ� �ڽŸ��� �⺻ �Ҵ�⸦ ���� ����


#include <iostream>
#include <vector>
using namespace std;

int main135()
{
	// ������ �����̳� : ���Լ����� ���� �����Ǵ� �ڷᱸ�� (vector, deque, list)
	// ���� �����̳� : Ư���� ���� ������ ���� �ִ� �ڷᱸ�� (set, multiset, map, multimap)

	// vector : �迭��� ������ �����̳�
	// push_back, pop_back, operator[], size ���� ����Լ��� ��������
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for (unsigned int i = 0; i < v.size(); ++i) {
		cout << v[i] << endl;						// v[i] �� i��° index�� ���� ��ȯ
	}

	return 0;
}