#pragma once

namespace STL229 
{
	// vector, list, map, set, string

	// std::vector �� new�� ������� ���� �� �ִ�
	void stdVector();
	void withoutNew();

	// ��ũ�帮��Ʈ : ���ӵ��� ���� ��带 �Ϸķ� �����Ѵ�
	// �߰� �� �����ϴ� ����� ����
	// list, deque, stack, queue
	void stdList();
	void stdDeque();
	void stdStack();
	void stdQueue();
	
	// map : ���� �˻� Ʈ��, �ڵ� ���� ����, log n, ���� �� ���������� �ʿ��� ��� ��� (������ �߿��� ���)
	// unordered_map : �ؽ� ���̺�, ���� ����, 1 ~ n, �˻��� ���԰� ������ ������ (������ �߿��� ���)
	void stdMap();
	void stdUnorderedMap();
	void stdMultimap();				// �ߺ�Ű ����
	void mapSorting();

	// set : �ߺ�����, �ڵ�����
	// unordered_set : �ߺ�����, ���� ����(�˻�, ����, ����)
	void stdSet();
	void stdUnorderedSet();
}