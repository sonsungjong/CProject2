#include "pch.h"
#include "p229_STL.h"
#include <Windows.h>
#include <vector>
#include <list>
#include <deque>
#include <stack>
#include <queue>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <concurrent_unordered_map.h>			// 다중 쓰레드 보호 unordered_map
#include <concurrent_unordered_set.h>
#include <concurrent_queue.h>
#include <concurrent_priority_queue.h>					// 느린 대신 (log n) 우선 순위가 중요한 작업에서 사용
#include <concurrent_vector.h>
#include <internal_concurrent_hash.h>

void STL229::stdVector()
{
	std::vector<int> vecNum;
	vecNum.push_back(1);
	vecNum.push_back(2);
	vecNum.resize(10, -1);				// 10개의 배열을 -1로 할당하여 생성

	for (size_t i = 0; i < vecNum.size(); ++i) {
		printf("%d ", vecNum[i]);
	}
	// 결과: 1 2 -1 -1 -1 -1 -1 -1 -1 -1

}

void STL229::withoutNew()
{
	// new를 사용하지 않고 동적할당하자
	const size_t ullBufferSize = 1024;

	// new 동적할당1
	unsigned char* pBuffer = new unsigned char[ullBufferSize] {0};
	delete[] pBuffer;

	// vector 동적할당2
	std::vector<unsigned char> vecBuffer;
	vecBuffer.resize(ullBufferSize, 0);
	
	for (unsigned char chBuf : vecBuffer) {
		printf("%c", chBuf);
	}
}

void STL229::stdList()
{
	// 양방향
	std::list<int> listNum;
	listNum.push_back(1);
	listNum.push_back(2);
	listNum.push_back(3);
	listNum.pop_back();
	listNum.push_front(4);

	for (int nValue : listNum) {
		printf("%d ", nValue);
	}
}

void STL229::stdDeque()
{
	// 양방향
	std::deque<int> dequeNum;
	dequeNum.push_back(1);
	dequeNum.push_back(2);
	dequeNum.push_back(3);
	dequeNum.pop_back();
	dequeNum.push_front(4);
	
	for (int nValue : dequeNum) {
		printf("%d ", nValue);
	}
}

void STL229::stdStack()
{
	// 단방향 FILO
	std::stack<int> stackNum;
	stackNum.push(1);
	stackNum.push(2);
	stackNum.push(3);

	while (!stackNum.empty()) {
		int nValue = stackNum.top();
		stackNum.pop();
		printf("%d ", nValue);
	}
}

void STL229::stdQueue()
{
	// 단방향 FIFO
	std::queue<int> queueNum;
	queueNum.push(1);
	queueNum.push(2);
	queueNum.push(3);

	while (!queueNum.empty()) {
		int nValue = queueNum.front();
		queueNum.pop();
		printf("%d ", nValue);
	}



}

void STL229::stdMap()
{

}

void STL229::stdUnorderedMap()
{

}

void STL229::stdMultimap()
{

}

void STL229::mapSorting()
{

}
