// vector : 시퀀스 컨테이너, 배열 기반 컨테이너, 임의 접근 반복자 제공, reverse 제공
// deque : 시퀀스 컨테이너, 배열 기반 컨테이너, 컨테이너 앞뒤로 추가제거 가능, 임의 접근 반복자 제공
// list : 시퀀스 컨테이너, sort()와 splice() 제공, 컨테이너 앞뒤로 추가제거 가능, reverse() 제공, 빠른 시간에 원소 삽입삭제 가능

// 10, 100, 20, 30, 40, 50, N
// size: 5, capacity : 10
// vector[], vector.at() : 값을 참조, at은 원소가 없으면 예외처리해주고, []는 빠른 대신 잘못 사용하면 프로그램 종료

// vector와 deque의 차이점 : vector : 뒤로만 추가삭제, deque: 앞뒤로 추가삭제
// vector와 list 차이점 : vector는 삽입하면 뒤로 밀어내는데(연속 메모리 기반), list는 삽입하면 노드 연결만 다시함(노드 기반)
// vector반복자 : 임의접근 반복자, list반복자 : 양방향 반복자

// vector와 list의 공통점 : 시퀀스 컨테이너, 원소를 추가하면서 공간을 늘려나감
// erase() : vector에서는 list에서는 
// insert() : vector에서는 삽입 후 뒤로 밀어내지만 list는 삽입 시 노드만 재연결