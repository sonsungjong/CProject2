#pragma once

namespace STL229 
{
	// vector, list, map, set, string

	// std::vector 로 new를 사용하지 않을 수 있다
	void stdVector();
	void withoutNew();

	// 링크드리스트 : 연속되지 않은 노드를 일렬로 연결한다
	// 추가 및 제거하는 비용이 적다
	// list, deque, stack, queue
	void stdList();
	void stdDeque();
	void stdStack();
	void stdQueue();
	
	// map : 이진 검색 트리, 자동 순차 정렬, log n, 정렬 및 순차접근이 필요한 경우 사용 (순서가 중요한 경우)
	// unordered_map : 해시 테이블, 임의 순서, 1 ~ n, 검색과 삽입과 삭제가 빠르다 (성능이 중요한 경우)
	void stdMap();
	void stdUnorderedMap();
	void stdMultimap();				// 중복키 가능
	void mapSorting();

	// set : 중복방지, 자동정렬
	// unordered_set : 중복방지, 빠른 성능(검색, 삽입, 삭제)
	void stdSet();
	void stdUnorderedSet();
}