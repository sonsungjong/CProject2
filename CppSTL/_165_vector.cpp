// vector : 시퀀스 컨테이너, 배열과 비슷함

// 템플릿 형식
/*
	template<typename T, typename Allocator = allocator<T>>
	class vector
	{
	};
*/

// 생성자 오버로딩
/*
	vector v : 빈 컨테이너가 만들어짐
	vector v(n) : 초기화된 n개의 원소를 갖는다.
	vector v(n, x) : x값으로 초기화된 n개의 원소를 갖는다.
	vector v(v2) : v는 v2의 복사본, 복사생성자 호출
	vector v(b,e) : [b,e)로 초기화된 원소를 갖는다.
*/

// 멤버 함수
/*
	assign(n,x) : x값으로 n개의 원소를 할당한다.
	assign(b,e) : 반복자 구간 [b,e)로 할당한다.
	at(i) : i번째 원소를 참조한다.
	back() : 마지막 원소를 참조한다.
	begin() : 벡터의 첫 원소를 가리키는 반복자, 해당 위치를 반환한다.
	capacity : 벡터에 할당된 공간의 크기를 반환한다.
	clear() : 모든 원소를 제거한다.
	empty() : 비어있는지 조사한다.
	end() : 벡터의 끝을 표식하는 반복자, 해당 위치를 반환한다.
	erase(p) : p 위치의 원소를 제거한다. 반환값은 다음 원소를 가리킨다.
	v.erase(b,e) : 반복자 구간 [b,e)의 모든 원소를 제거한다. 반환값은 다음 원소
	front() : 첫번째 원소를 참조
	insert(p,x) : p위치에 x를 삽입한다. 반환값은 삽입한 위치를 가리키는 반복자.
	insert(p, n, x) : p위치에 n개의 x값을 삽입
	insert(p, b, e) : p위치에 반복자 구간 [b,e) 원소를 삽입한다
	max_size() : 최대 원소의 갯수를 반환
	pop_back() : 마지막 원소를 제거한다.
	push_back(x) : 마지막 위치에 x를 추가한다.
	rbegin() : 역순차열의 첫 원소를 가리키는 반복자를 반환
	rend() : 역순차열의 끝을 표식하는 반복자
	reserve(n) : n개의 원소를 저장할 공간을 예약
	resize(n) : 벡터의 크기를 n으로 변경하고 확장된 공간의 값은 기본값으로 초기화한다.
	resize(n, x) : 벡터의 크기를 n으로 변경하고 확장된 공간의 값을 x로 초기화한다.
	size() : 원소의 갯수 반환
	swap(v2) : 벡터와 다른벡터v2의 원소들을 서로 바꾼다.

	v[i] : i번째 위치 참조
	== : bool값 반환
	!= : bool값 반환
	> : 
	< : 
	<= : 
	>= : 

*/