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
	std::map<int, const char*> mapTest;
	mapTest.insert(std::make_pair(0, "zero"));
	mapTest.insert(std::make_pair(0, "zero2"));			// insert는 덮어쓰기를 하지않음, 무시됨
	mapTest.insert(std::make_pair(1, "one"));
	mapTest[1] = "one2";												// operator[] 는 덮어쓰기를 함, 변경됨
	
	for (auto iter = mapTest.begin(); iter != mapTest.end(); ++iter)
	{
		printf("%d-%s ", iter->first, iter->second);
	}

	printf("\n");
	mapTest.erase(0);
	for (auto iter = mapTest.begin(); iter != mapTest.end(); ++iter)
	{
		printf("%d-%s ", iter->first, iter->second);
	}
}

void STL229::stdUnorderedMap()
{
	std::unordered_map<int, const char*> umapTest;
	umapTest.insert(std::make_pair(0, "zero"));
	umapTest.insert(std::make_pair(0, "zero2"));			// 덮어쓰기 불가
	umapTest.insert(std::make_pair(1, "one"));
	umapTest[1] = "one2";					// 덮어쓰기 가능
	umapTest.insert(std::make_pair(2, "짝수"));
	umapTest.insert(std::make_pair(3, "홀수"));
	umapTest.insert(std::make_pair(4, "짝수"));
	umapTest.insert(std::make_pair(5, "홀수"));
	umapTest[6] = "짝수";

	// 일반 for (반복자 사용)
	for (auto iter = umapTest.begin(); iter != umapTest.end(); ++iter)
	{
		printf("%d-%s ", iter->first, iter->second);
	}

	printf("\n");
	umapTest.erase(0);
	// 범위기반 for
	for (const auto& pair : umapTest)
	{
		printf("%d-%s ", pair.first, pair.second);
	}
	printf("\n");

	// 특정 키 찾기
	int nKey = 1;
	auto it = umapTest.find(nKey);
	if (it != umapTest.end())
	{
		printf("Key %d, Value %s", nKey, it->second);
	}
	else {
		printf("Key %d 는 존재하지 않습니다", nKey);
	}

	printf("\n");

	// 특정 키의 소속 찾기 (값에 소속을 넣어놓기)
	nKey = 5;
	if (umapTest[nKey] == "짝수")
	{
		printf("해당 키는 %s 입니다", umapTest[nKey]);
	}
	else if (umapTest[nKey] == "홀수")
	{
		printf("해당 키는 %s 입니다", umapTest[nKey]);
	}
	else {
		printf("일치하는 값이 없습니다");
	}
}

void STL229::stdMultimap()
{
	std::multimap<int, const char*> mapTest;				// 중복키 허용
	mapTest.insert(std::make_pair(0, "zero"));
	mapTest.insert(std::make_pair(0, "zero2"));
	mapTest.insert(std::make_pair(1, "one"));

	for (auto iter = mapTest.begin(); iter != mapTest.end(); ++iter)
	{
		printf("%d-%s ", iter->first, iter->second);
	}

	printf("\n");
	mapTest.erase(0);

	for (auto iter = mapTest.begin(); iter != mapTest.end(); ++iter)
	{
		printf("%d-%s ", iter->first, iter->second);
	}
}

void STL229::mapSorting()
{
	// map은 자동으로 정렬해준다
	std::map<int, const char*> mapTest;
	mapTest.insert(std::make_pair(5, "five"));
	mapTest.insert(std::make_pair(2, "two"));
	mapTest.insert(std::make_pair(3, "three"));
	mapTest[4] = "four";

	for (auto iter = mapTest.begin(); iter != mapTest.end(); ++iter)
	{
		printf("%d-%s ", iter->first, iter->second);
	}

	printf("\n");
	printf("%d-%s ", mapTest.begin()->first, mapTest.begin()->second);					// 시작노드
	printf("%d-%s ", mapTest.rbegin()->first, mapTest.rbegin()->second);				// 끝전노드 (끝노드는 nul)
}

void STL229::stdSet()
{
	std::set<int> setTest;					// 중복방지, 자동정렬
	setTest.insert(4);
	setTest.insert(1);
	setTest.insert(7);
	setTest.insert(2);
	setTest.insert(2);
	setTest.insert(2);

	// 일반 for
	for (auto iter = setTest.begin(); iter != setTest.end(); ++iter)
	{
		printf("%d ", *iter);
	}
	printf("\n");

	// 범위기반 for
	for (const int& nValue : setTest) 
	{
		printf("%d ", nValue);
	}
}

void STL229::stdUnorderedSet()
{
	std::unordered_set<int> usetTest;				// 중복방지
	usetTest.insert(4);
	usetTest.insert(1);
	usetTest.insert(7);
	usetTest.insert(2);
	usetTest.insert(2);
	usetTest.insert(2);

	for (const int& nValue : usetTest)
	{
		printf("%d ", nValue);
	}
}
