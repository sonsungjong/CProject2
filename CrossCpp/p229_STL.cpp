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
#include <concurrent_unordered_map.h>			// ���� ������ ��ȣ unordered_map
#include <concurrent_unordered_set.h>
#include <concurrent_queue.h>
#include <concurrent_priority_queue.h>					// ���� ��� (log n) �켱 ������ �߿��� �۾����� ���
#include <concurrent_vector.h>
#include <internal_concurrent_hash.h>

void STL229::stdVector()
{
	std::vector<int> vecNum;
	vecNum.push_back(1);
	vecNum.push_back(2);
	vecNum.resize(10, -1);				// 10���� �迭�� -1�� �Ҵ��Ͽ� ����

	for (size_t i = 0; i < vecNum.size(); ++i) {
		printf("%d ", vecNum[i]);
	}
	// ���: 1 2 -1 -1 -1 -1 -1 -1 -1 -1

}

void STL229::withoutNew()
{
	// new�� ������� �ʰ� �����Ҵ�����
	const size_t ullBufferSize = 1024;

	// new �����Ҵ�1
	unsigned char* pBuffer = new unsigned char[ullBufferSize] {0};
	delete[] pBuffer;

	// vector �����Ҵ�2
	std::vector<unsigned char> vecBuffer;
	vecBuffer.resize(ullBufferSize, 0);
	
	for (unsigned char chBuf : vecBuffer) {
		printf("%c", chBuf);
	}
}

void STL229::stdList()
{
	// �����
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
	// �����
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
	// �ܹ��� FILO
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
	// �ܹ��� FIFO
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
	mapTest.insert(std::make_pair(0, "zero2"));			// insert�� ����⸦ ��������, ���õ�
	mapTest.insert(std::make_pair(1, "one"));
	mapTest[1] = "one2";												// operator[] �� ����⸦ ��, �����
	
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
	umapTest.insert(std::make_pair(0, "zero2"));			// ����� �Ұ�
	umapTest.insert(std::make_pair(1, "one"));
	umapTest[1] = "one2";					// ����� ����
	umapTest.insert(std::make_pair(2, "¦��"));
	umapTest.insert(std::make_pair(3, "Ȧ��"));
	umapTest.insert(std::make_pair(4, "¦��"));
	umapTest.insert(std::make_pair(5, "Ȧ��"));
	umapTest[6] = "¦��";

	// �Ϲ� for (�ݺ��� ���)
	for (auto iter = umapTest.begin(); iter != umapTest.end(); ++iter)
	{
		printf("%d-%s ", iter->first, iter->second);
	}

	printf("\n");
	umapTest.erase(0);
	// ������� for
	for (const auto& pair : umapTest)
	{
		printf("%d-%s ", pair.first, pair.second);
	}
	printf("\n");

	// Ư�� Ű ã��
	int nKey = 1;
	auto it = umapTest.find(nKey);
	if (it != umapTest.end())
	{
		printf("Key %d, Value %s", nKey, it->second);
	}
	else {
		printf("Key %d �� �������� �ʽ��ϴ�", nKey);
	}

	printf("\n");

	// Ư�� Ű�� �Ҽ� ã�� (���� �Ҽ��� �־����)
	nKey = 5;
	if (umapTest[nKey] == "¦��")
	{
		printf("�ش� Ű�� %s �Դϴ�", umapTest[nKey]);
	}
	else if (umapTest[nKey] == "Ȧ��")
	{
		printf("�ش� Ű�� %s �Դϴ�", umapTest[nKey]);
	}
	else {
		printf("��ġ�ϴ� ���� �����ϴ�");
	}
}

void STL229::stdMultimap()
{
	std::multimap<int, const char*> mapTest;				// �ߺ�Ű ���
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
	// map�� �ڵ����� �������ش�
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
	printf("%d-%s ", mapTest.begin()->first, mapTest.begin()->second);					// ���۳��
	printf("%d-%s ", mapTest.rbegin()->first, mapTest.rbegin()->second);				// ������� (������ nul)
}

void STL229::stdSet()
{
	std::set<int> setTest;					// �ߺ�����, �ڵ�����
	setTest.insert(4);
	setTest.insert(1);
	setTest.insert(7);
	setTest.insert(2);
	setTest.insert(2);
	setTest.insert(2);

	// �Ϲ� for
	for (auto iter = setTest.begin(); iter != setTest.end(); ++iter)
	{
		printf("%d ", *iter);
	}
	printf("\n");

	// ������� for
	for (const int& nValue : setTest) 
	{
		printf("%d ", nValue);
	}
}

void STL229::stdUnorderedSet()
{
	std::unordered_set<int> usetTest;				// �ߺ�����
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
