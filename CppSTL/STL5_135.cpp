// page 131
// STL : 표준 C++ 라이브러리의 일부분, Standard Template Library
// 자료구조 + 알고리즘 + 반복자를 템플릿으로 제공하는 라이브러리

// 1. 컨테이너(Container) : 객체를 저장하는 객체, 컬렉션 또는 자료구조라고도 불림
// 2. 반복자(Iterator) : 포인터와 비슷한 개념으로 컨테이너의 원소를 가리키고, 가리키는 원소에 접근하여 다음 원소를 가리키게 하는 기능을 한다.
// 3. 알고리즘(Algorithm) : 정렬, 삭제, 검색, 연산 등 일반화된 방법을 제공하는 함수 템플릿
// 4. 함수 객체(함수자, Function Object) : 함수처럼 동작하는 객체, operator() 를 오버로딩하여 정의한다. 자료구조 및 알고리즘의 클라이언트 정책을 반영한다.
// 5. 어댑터(Adaptor) : 인터페이스를 변경해 새로운 인터페이스를 갖는 구성 요소로 변경함
// 6. 할당기(Allocator) : 메모리 할당 정책을 캡슐화한 클래스 객체, 모든 컨테이너는 자신만의 기본 할당기를 갖고 있음


#include <iostream>
#include <vector>
using namespace std;

int main135()
{
	// 시퀀스 컨테이너 : 삽입순서에 따라 결정되는 자료구조 (vector, deque, list)
	// 연관 컨테이너 : 특정한 정렬 기준을 갖고 있는 자료구조 (set, multiset, map, multimap)

	// vector : 배열기반 시퀀스 컨테이너
	// push_back, pop_back, operator[], size 등을 멤버함수로 갖고있음
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for (unsigned int i = 0; i < v.size(); ++i) {
		cout << v[i] << endl;						// v[i] 는 i번째 index의 값을 반환
	}

	return 0;
}