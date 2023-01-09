#include <cstdio>
#include <set>
using namespace std;

// 연관 컨테이너 : set, map, multiset, multimap
// 중복X, 순서X : set, map
// 중복허용 : multiset, multimap
// 균형 이진 트리

// set
void main251()
{
	set<int> s;
	
	// 멤버 함수
	/*
		clear() : 모든 원소 제거
		count(k) : k의 갯수를 반환
		empty() : 비었는지 조사
		erase(k) : 원소 제거
		find(k) : 원소 위치 반환
		insert(k) : 원소 삽입
		size() : 원소 갯수
		swap(s2) : 교환
	*/
	
	// set은 insert를 통해 값을 대입
	s.insert(50);
	s.insert(30);
	s.insert(80);
	s.insert(40);
	s.insert(10);
	s.insert(70);
	s.insert(90);

	set<int>::iterator set_iter;				// 정렬 기준이 less인 양방향 반복자
	for (set_iter = s.begin(); set_iter != s.end(); ++set_iter)
	{
		printf("%d ", *set_iter);
	}
	printf("\n");

	// 중복이 불가능하기 때문에 실패
	s.insert(50);
	s.insert(50);

	for (set_iter = s.begin(); set_iter != s.end(); ++set_iter)
	{
		printf("%d ", *set_iter);
	}
	printf("\n");

	printf("======================");
	// insert 성공유무 반환
	pair<set<int>::iterator, bool> pr;

	pr = s.insert(20);
	if (pr.second == true) {
		printf("%d 삽입 성공", *pr.first);
	}
	else {
		printf("%d가 이미 있습니다. 삽입 실패!", *pr.first);
	}
	printf("\n");

	pr = s.insert(20);
	if (pr.second == true) {
		printf("%d 삽입 성공", *pr.first);
	}
	else {
		printf("%d가 이미 있습니다. 삽입 실패!", *pr.first);
	}
	printf("\n");
}